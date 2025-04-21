#pragma once

#include <optional>
#include <unordered_map>

namespace matchmaker::core {

class Game; class User;

class RatingMapObserver;

using Rating = int;

class RatingMapPerGame {
public:
    explicit RatingMapPerGame(const Game *game, RatingMapObserver *observer = nullptr);

    std::optional<Rating> get_rating(const User *user) const;
    Rating get_rating(const User *user);

    void set_rating(const User *user, Rating rating);
    void change_rating(const User *user, Rating rating_diff);

    void rem_user(const User *user);

    inline void set_observer(RatingMapObserver *observer) noexcept
    {
        this->observer = observer;
    }

private:
    std::unordered_map<const User *, Rating> rating_by_user;
    const Game *game = nullptr;
    RatingMapObserver *observer = nullptr;
};

class RatingMap {
public:
    std::optional<Rating> get_rating(const Game *game, const User *user) const;
    Rating get_rating(const Game *game, const User *user);

    void set_rating(const Game *game, const User *user, Rating rating);
    void change_rating(const Game *game, const User *user, Rating rating_diff);

    void rem_user(const User *user);

    RatingMapPerGame borrow_rating_map_for_game(const Game *game);
    void return_rating_map_for_game(const Game *game, RatingMapPerGame&& rating_map);

    void set_observer(RatingMapObserver *observer) noexcept;

private:
    RatingMapPerGame& get_or_create_rating_per_game(const Game *game);

    std::unordered_map<const Game *, RatingMapPerGame> rating_by_user_by_game;
    RatingMapObserver *observer = nullptr;
};

class RatingMapObserver {
public:
    virtual void on_rating_change(const Game& game, const User& user, Rating rating) = 0;
};

}
