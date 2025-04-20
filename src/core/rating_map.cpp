#include "core/rating_map.h"

namespace matchmaker::core {

std::optional<Rating> RatingMap::get_rating(
        const Game& game, const User& user) const
{
    auto rating_by_user_it = rating_by_user_by_game.find(&game);
    if (rating_by_user_it == rating_by_user_by_game.end())
        return std::nullopt;

    auto& rating_by_user = rating_by_user_it->second;
    auto rating_it = rating_by_user.find(&user);
    if (rating_it == rating_by_user.end())
        return std::nullopt;

    const Rating rating = rating_it->second;
    return rating;
}

Rating RatingMap::get_rating(const Game& game, const User& user)
{
    return rating_by_user_by_game[&game][&user];
}

void RatingMap::set_rating(const Game& game, const User& user, Rating rating)
{
    Rating& rating_ref = rating_by_user_by_game[&game][&user];
    if (rating_ref != rating) {
        rating_ref = rating;
        if (observer)
            observer->notify_rating_change(game, user, rating_ref);
    }
}

void RatingMap::change_rating(const Game& game, const User& user, Rating rating_diff)
{
    if (rating_diff != 0) {
        Rating& rating_ref = rating_by_user_by_game[&game][&user];
        rating_ref += rating_diff;
        if (observer)
            observer->notify_rating_change(game, user, rating_ref);
    }
}

void RatingMap::rem_user(const User& user)
{
    for (auto&& [game, rating_by_user] : rating_by_user_by_game)
        rating_by_user.erase(&user);
}

void RatingMap::rem_game(const Game& game)
{
    rating_by_user_by_game.erase(&game);
}

}
