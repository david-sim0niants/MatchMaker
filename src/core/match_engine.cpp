#include "core/match_engine.h"
#include "misc/printing.h"

namespace matchmaker::core {

MatchEngine::MatchEngine(
        misc::PRNG& prng,
        Waiter& waiter,
        UserRatingObserver *observer,
        PlayerObserver *player_observer) :
    prng(prng),
    waiter(waiter),
    rating_map_observer(observer),
    player_observer(player_observer),
    rating_map(observer ? &rating_map_observer : nullptr)
{
}

void MatchEngine::run()
{
    timeline.run();
}

void MatchEngine::keep_alive()
{
    timeline.sync_call([this]{ sentinel_event = Timeline::call_at(Time::max(), []{});});
}

void MatchEngine::let_die()
{
    timeline.sync_call([this]{ Timeline::cancel(sentinel_event); });
}

Player& MatchEngine::new_player_for(const User& user)
{
    auto player_it = player_by_user.find(&user);
    if (player_it != player_by_user.end())
        throw MatchEngineException(misc::stringify(
                "user '", user.get_username(), "' already playing"));

    return *(player_by_user[&user] =
             std::make_unique<Player>(user, mediator, prng, player_observer));
}

std::unique_ptr<Player> MatchEngine::del_player_of(const User& user)
{
    auto player_it = player_by_user.find(&user);
    if (player_it == player_by_user.end())
        throw MatchEngineException(misc::stringify(
                    "user '", user.get_username(), "' was not playing"));
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
