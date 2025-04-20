#pragma once

#include <optional>
#include <unordered_map>

namespace matchmaker::core {

class Game; class User;

class RatingMapObserver;

using Rating = int;

class RatingMap {
public:
    std::optional<Rating> get_rating(const Game& game, const User& user) const;
    Rating get_rating(const Game& game, const User& user);
    void set_rating(const Game& game, const User& user, Rating rating);
    void change_rating(const Game& game, const User& user, Rating rating_diff);

    void set_observer(RatingMapObserver *observer)
    {
        this->observer = observer;
    }

private:
    std::unordered_map<const Game *, std::unordered_map<const User *, Rating>>
        rating_by_user_by_game;
    RatingMapObserver *observer;
};

class RatingMapObserver {
public:
    virtual void notify_rating_change(const Game& game, const User& user, Rating rating) = 0;
};

}
