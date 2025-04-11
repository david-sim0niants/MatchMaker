#pragma once

#include <optional>
#include <unordered_map>

namespace matchmaker::core {

class Game; class Player;

class RatingMapObserver;

using Rating = int;

class RatingMap {
public:
    explicit RatingMap(RatingMapObserver *observer = nullptr);

    std::optional<Rating> get_rating(const Game& game, const Player& player) const;
    Rating get_rating(const Game& game, const Player& player);
    void set_rating(const Game& game, const Player& player, Rating rating);
    void change_rating(const Game& game, const Player& player, Rating rating_diff);

private:
    std::unordered_map<const Game *, std::unordered_map<const Player *, Rating>>
        rating_by_player_by_game;
    RatingMapObserver *observer;
};

class RatingMapObserver {
public:
    virtual void notify_rating_change(const Game& game, const Player& player, Rating rating) = 0;
};

}
