#include "core/rating_map.h"

#include <utility>

namespace matchmaker::core {

RatingMapPerGame::RatingMapPerGame(const Game *game, RatingMapObserver *observer) :
    game(game), observer(observer)
{
}

std::optional<Rating> RatingMapPerGame::get_rating(const User *user) const
{
    auto rating_it = rating_by_user.find(user);
    if (rating_it == rating_by_user.end())
        return std::nullopt;

    const Rating rating = rating_it->second;
    return rating;
}

Rating RatingMapPerGame::get_rating(const User *user)
{
    return rating_by_user[user];
}

void RatingMapPerGame::set_rating(const User *user, Rating rating)
{
    Rating& rating_ref = rating_by_user[user];
    if (rating_ref != rating) {
        rating_ref = rating;
        if (observer)
            observer->on_rating_change(*game, *user, rating_ref);
    }
}

void RatingMapPerGame::change_rating(const User *user, Rating rating_diff)
{
    Rating& rating_ref = rating_by_user[user];
    rating_ref += rating_diff;
    if (observer)
        observer->on_rating_change(*game, *user, rating_ref);
}

void RatingMapPerGame::rem_user(const User *user)
{
    rating_by_user.erase(user);
}

std::optional<Rating> RatingMap::get_rating(
        const Game *game, const User *user) const
{
    auto rating_by_user_it = rating_by_user_by_game.find(game);
    if (rating_by_user_it == rating_by_user_by_game.end())
        return std::nullopt;

    RatingMapPerGame const& rating_by_user = rating_by_user_it->second;
    return rating_by_user.get_rating(user);
}

Rating RatingMap::get_rating(const Game *game, const User *user)
{
    return get_or_create_rating_per_game(game).get_rating(user);
}

void RatingMap::set_rating(const Game *game, const User *user, Rating rating)
{
    get_or_create_rating_per_game(game).set_rating(user, rating);
}

void RatingMap::change_rating(const Game *game, const User *user, Rating rating_diff)
{
    if (rating_diff != 0)
        get_or_create_rating_per_game(game).change_rating(user, rating_diff);
}

void RatingMap::rem_user(const User *user)
{
    for (auto&& [game, rating_by_user] : rating_by_user_by_game)
        rating_by_user.rem_user(user);
}

const RatingMapPerGame *RatingMap::get_rating_map_for_game(const Game *game) const
{
    auto rating_by_user_it = rating_by_user_by_game.find(game);
    if (rating_by_user_it == rating_by_user_by_game.end())
        return nullptr;
    else
        return &rating_by_user_it->second;
}

RatingMapPerGame& RatingMap::get_rating_map_for_game(const Game *game)
{
    return get_or_create_rating_per_game(game);
}

void RatingMap::set_observer(RatingMapObserver *observer) noexcept
{
    this->observer = observer;
    for (auto&& [game, rating_by_user] : rating_by_user_by_game)
        rating_by_user.set_observer(observer);
}

RatingMapPerGame& RatingMap::get_or_create_rating_per_game(const Game *game)
{
    auto it = rating_by_user_by_game.find(game);
    if (it == rating_by_user_by_game.end())
        it = rating_by_user_by_game.emplace_hint(it, game, RatingMapPerGame(game, observer));
    return it->second;
}

}
