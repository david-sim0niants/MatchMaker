#pragma once

#include "core/player.h"
#include "core/match.h"
#include "core/match_arranger.h"
#include "core/rating_map.h"

#include <unordered_map>

namespace matchmaker::core {

class MatchMediator : public PlayerEndpoint, public MatchEndpoint {
    using Rating = MatchArranger::Rating;

public:
    explicit MatchMediator(RatingMap& rating_map);

    void request_match(Player& player, Game& game) override;
    void withdraw_match(const Player& player) override;
    void notify_match_finished(Match& match, GameWinner winner) override;

private:
    Rating get_player_rating_for_game(const Game& game, const Player& player);
    void increment_player_rating_for_game(const Game& game, const Player& player);

    void add_match(std::unique_ptr<Match>&& match);
    void remove_match(const Match& match);

    RatingMap& rating_map;
    MatchArranger match_arranger;
    std::unordered_map<const Match *, std::unique_ptr<Match>> match_container;
};

}
