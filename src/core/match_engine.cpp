#include "core/match_engine.h"
#include "misc/printing.h"

namespace matchmaker::core {

MatchEngine::MatchEngine(misc::PRNG& prng, Waiter& waiter) :
    prng(prng),
    timeline(waiter)
{
}

RatingMap MatchEngine::run(RatingMap&& rating_map)
{
    std::swap(rating_map, context.rating_map);
    timeline.run();
    std::swap(rating_map, context.rating_map);
    return std::move(rating_map);
}

void MatchEngine::keep_alive()
{
    timeline.post([this]{ sentinel_event = Timeline::call_at(Time::max(), []{}); });
}

void MatchEngine::let_die()
{
    timeline.post([this]{ Timeline::cancel(sentinel_event); });
}

Player& MatchEngine::new_player_for(const User *user)
{
    auto player_it = player_by_user.find(user);
    if (player_it != player_by_user.end())
        throw MatchEngineException(misc::stringify(
                "user '", user->get_username(), "' already playing"));

    return *(player_by_user[user] =
             std::make_unique<Player>(user, mediator, prng, player_observer));
}

std::unique_ptr<Player> MatchEngine::del_player_of(const User *user)
{
    auto player_it = player_by_user.find(user);
    if (player_it == player_by_user.end())
        throw MatchEngineException(misc::stringify(
                    "user '", user->get_username(), "' was not playing"));
    auto player = std::move(player_it->second);
    player_by_user.erase(player_it);
    return player;
}

void MatchEngine::del_all_players()
{
    for (auto&& [user, player] : player_by_user)
        player->deinit();
    player_by_user.clear();
}

}
