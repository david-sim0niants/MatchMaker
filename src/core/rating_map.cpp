#include "core/rating_map.h"

namespace matchmaker::core {

RatingMap::RatingMap(RatingMapObserver *observer) :
    observer(observer)
{
}

std::optional<Rating> RatingMap::get_rating(
        const Game& game, const Player& player) const
{
    auto rating_by_player_it = rating_by_player_by_game.find(&game);
    if (rating_by_player_it == rating_by_player_by_game.end())
        return std::nullopt;

    auto& rating_by_player = rating_by_player_it->second;
    auto rating_it = rating_by_player.find(&player);
    if (rating_it == rating_by_player.end())
        return std::nullopt;

    const Rating rating = rating_it->second;
    return rating;
}

Rating RatingMap::get_rating(const Game& game, const Player& player)
{
    return rating_by_player_by_game[&game][&player];
}

void RatingMap::set_rating(const Game& game, const Player& player, Rating rating)
{
    Rating& rating_ref = rating_by_player_by_game[&game][&player];
    if (rating_ref != rating) {
        rating_ref = rating;
        if (observer)
            observer->notify_rating_change(game, player, rating_ref);
    }
}

void RatingMap::change_rating(const Game& game, const Player& player, Rating rating_diff)
{
    if (rating_diff != 0) {
        Rating& rating_ref = rating_by_player_by_game[&game][&player];
        rating_ref += rating_diff;
        if (observer)
            observer->notify_rating_change(game, player, rating_ref);
    }
}

}
