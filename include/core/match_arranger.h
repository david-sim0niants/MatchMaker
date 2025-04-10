#pragma once

#include "core/exception.h"
#include "player.h"
#include "match.h"

#include <map>
#include <optional>
#include <unordered_map>

namespace matchmaker::core {

class MatchArranger {
    using MatchByRating = std::multimap<int, Match>;
    using MatchByRatingByGame = std::unordered_map<const Game *, MatchByRating>;
    using MatchByRatingIt = MatchByRating::iterator;
    using MatchByRatingByGameIt = MatchByRatingByGame::iterator;

public:
    std::optional<Match> find_or_request_match(Player& player, int player_rating, const Game& game);
    void withdraw_match(const Player& player);

    inline bool has_arranged_match_for(const Player& player) const
    {
        return get_player_game(player) != nullptr;
    }

    const Game *get_player_game(const Player& player) const;

    static constexpr int lower_rating_diff_thresh = 1,
                        higher_rating_diff_thresh = 3;

private:
    static MatchByRatingIt
        try_find_fairest_match(int player_rating, MatchByRating& match_by_rating);

    void arrange_new_match(Player& player, int rating, const Game& game);
    Match release_match(Player& player, const Game& game, MatchByRatingIt rating_it);

    MatchByRatingByGame match_by_rating_by_game;
    std::unordered_map<const Player *, const Game *> game_by_player;
};

class MatchArrangerException : public Exception {
public:
    using Exception::Exception;
};

}
