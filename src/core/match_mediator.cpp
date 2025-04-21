#include "core/match_mediator.h"
#include "core/rating_map.h"

namespace matchmaker::core {

MatchMediator::MatchMediator(RatingMap& rating_map, Timeline& timeline) :
    rating_map(rating_map), timeline(timeline)
{
}

void MatchMediator::request_match(Player& player, const Game& game)
{
    Rating rating = get_player_rating_for_game(game, player);
    auto match = match_arranger.find_or_request_match(player, rating, game);
    if (match) {
        match->start(*this);
        add_match(std::move(match));
    }
}

void MatchMediator::withdraw_match(const Player& player)
{
    match_arranger.withdraw_match(player);
}

void MatchMediator::notify_match_finished(Match& match, Player *winner)
{
    if (winner) {
        const Game& game = match.get_game();
        increment_player_rating_for_game(game, *winner);
    }
    remove_match(match);
}

Timeline& MatchMediator::get_timeline() const noexcept
{
    return timeline;
}

Rating MatchMediator::get_player_rating_for_game(const Game& game, const Player& player) const
{
    return static_cast<const RatingMap&>(rating_map).
        get_rating(&game, &player.get_user()).value_or(0);
}

void MatchMediator::increment_player_rating_for_game(const Game& game, const Player& player)
{
    return rating_map.change_rating(&game, &player.get_user(), Rating(+1));
}

void MatchMediator::add_match(std::unique_ptr<Match>&& match)
{
    match_container.emplace(match.get(), std::move(match));
}

void MatchMediator::remove_match(const Match& match)
{
    match_container.erase(&match);
}

}
