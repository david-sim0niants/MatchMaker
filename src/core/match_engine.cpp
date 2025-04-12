#include "core/match_engine.h"
#include "misc/printing.h"
#include <iostream>

namespace matchmaker::core {

MatchEngine::RatingMapObserverAdapter::
    RatingMapObserverAdapter(UserRatingObserver *user_rating_observer) :
        user_rating_observer(user_rating_observer)
{
}

void MatchEngine::RatingMapObserverAdapter::notify_rating_change(
        const Game& game, const Player& player, Rating rating)
{
    user_rating_observer->notify_rating_change(game, player.get_user(), static_cast<int>(rating));
}

MatchEngine::MatchEngine(Waiter& waiter, UserRatingObserver *observer) :
    waiter(waiter),
    rating_map_observer_adapter(observer),
    rating_map(observer ? &rating_map_observer_adapter : nullptr)
{
}

void MatchEngine::run()
{
    timeline.run();
}

void MatchEngine::add_user(const User& user)
{
    timeline.join([&player = new_player_for(user)]() mutable { player.init(); });
}

void MatchEngine::rem_user(const User& user)
{
    timeline.join([player = del_player_of(user)]() mutable { player.deinit(); });
}

Player& MatchEngine::new_player_for(const User& user)
{
    auto player_it = player_by_user.find(&user);
    if (player_it != player_by_user.end())
        throw MatchEngineException(misc::stringify(
                "user '", user.get_username(), "' already playing"));

    return *player_by_user.emplace(&user, new Player(user, mediator)).first->second;
}

Player MatchEngine::del_player_of(const User& user)
{
    auto player_it = player_by_user.find(&user);
    if (player_it == player_by_user.end())
        throw MatchEngineException(misc::stringify(
                    "user '", user.get_username(), "' was not playing"));
    auto player = std::move(*player_it->second);
    delete player_it->second;
    player_by_user.erase(player_it);
    return std::move(player);
}

}
