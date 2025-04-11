#pragma once

#include "exception.h"
#include "player.h"
#include "match.h"

#include <memory>
#include <map>
#include <unordered_map>

namespace matchmaker::core {

class MatchArranger {
public:
    using Rating = int;

private:
    struct RatingKey {
        Rating rating;
        const Player *player;
    };

    struct RatingKeyCompare {
        using is_transparent = void;

        bool operator()(const RatingKey& lhs, const RatingKey& rhs) const noexcept
        {
            return std::tie(lhs.rating, lhs.player) < std::tie(rhs.rating, rhs.player);
        }

        bool operator()(const RatingKey& lhs, Rating rhs) const noexcept
        {
            return lhs.rating < rhs;
        }

        bool operator()(Rating lhs, const RatingKey& rhs) const noexcept
        {
            return lhs < rhs.rating;
        }
    };


    using MatchByRating = std::map<RatingKey, Match *, RatingKeyCompare>;
    using MatchByRatingByGame = std::unordered_map<const Game *, MatchByRating>;
    using MatchByRatingIt = MatchByRating::iterator;
    using MatchByRatingByGameIt = MatchByRatingByGame::iterator;

    struct MatchRequest {
        Rating rating;
        std::unique_ptr<Match> inactive_match;

        inline const Game& get_game()
        {
            return inactive_match->get_game();
        }

        MatchRequest(Player& player, Rating rating, const Game& game) :
            rating(rating), inactive_match(std::make_unique<Match>(game))
        {
            inactive_match->add_player(player);
        }
    };

public:
    std::unique_ptr<Match>
        find_or_request_match(Player& player, Rating player_rating, const Game& game);

    void withdraw_match(const Player& player);

    inline bool has_arranged_match_for(const Player& player) const
    {
        return get_match_request(player) != nullptr;
    }

    static constexpr Rating lower_rating_diff_thresh = 1,
                           higher_rating_diff_thresh = 3;

private:
    MatchRequest * get_match_request(const Player& player);
    const MatchRequest * get_match_request(const Player& player) const;


    void arrange_new_match(Player& player, Rating rating, const Game& game);
    std::unique_ptr<Match>
        release_match(Player& player, const Game& game, MatchByRatingIt match_it);

    void remove_match(const Player& player);
    MatchRequest remove_match_request(const Player& player);

    static MatchByRatingIt try_find_fairest_match(Rating rating, MatchByRating& match_by_rating);

    std::unordered_map<const Player *, MatchRequest> match_request_by_player;
    MatchByRatingByGame match_by_rating_by_game;
};

class MatchArrangerException : public Exception {
public:
    using Exception::Exception;
};

}
