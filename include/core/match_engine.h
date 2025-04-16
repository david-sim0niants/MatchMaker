#pragma once

#include "core/player.h"
#include "core/waiter.h"
#include "core/timeline.h"
#include "core/match_mediator.h"
#include "core/rating_map.h"
#include "core/user.h"
#include "core/user_observers.h"
#include "exception.h"
#include "misc/prng.h"

#include <future>
#include <type_traits>

namespace matchmaker::core {

class MatchEngine {
public:
    explicit MatchEngine(misc::PRNG& prng, Waiter& waiter,
            UserRatingObserver *rating_observer = nullptr,
            PlayerObserver *player_observer = nullptr);

    MatchEngine(const MatchEngine&) = delete;
    MatchEngine& operator=(const MatchEngine&) = delete;

    MatchEngine(MatchEngine&&) = delete;
    MatchEngine& operator=(MatchEngine&&) = delete;

    void run();

    inline std::future<void> run_async()
    {
        return std::async(&MatchEngine::run, this);
    }

    void keep_alive();
    void let_die();

    template<typename F = void(*)(), std::enable_if_t<std::is_invocable_v<F>, int> = 0>
    void add_user(const User& user, F on_add = []{})
    {
        timeline.post([this, &user, on_add]{ new_player_for(user).init(); on_add(); });
    }

    template<typename F = void(*)(), std::enable_if_t<std::is_invocable_v<F>, int> = 0>
    void rem_user(const User& user, F on_rem = []{})
    {
        timeline.post([this, &user, on_rem]{ del_player_of(user)->deinit(); on_rem(); });
    }

    template<typename F = void(*)(), std::enable_if_t<std::is_invocable_v<F>, int> = 0>
    void rem_all_users(F on_rem = []{})
    {
        timeline.post([this, on_rem]{ del_all_players(); });
    }

private:
    Player& new_player_for(const User& user);
    std::unique_ptr<Player> del_player_of(const User& user);
    void del_all_players();

    misc::PRNG& prng;

    Waiter& waiter;
    Timeline timeline {waiter};
    EventHandle sentinel_event;

    RatingMapToUserRatingObserverAdapter rating_map_observer;
    PlayerObserver *player_observer;

    RatingMap rating_map;
    MatchMediator mediator {rating_map, timeline};
    std::unordered_map<const User *, std::unique_ptr<Player>> player_by_user;
};

class MatchEngineException : public Exception {
public:
    using Exception::Exception;
};

}
