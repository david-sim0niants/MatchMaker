#pragma once

#include "core/player.h"
#include "core/waiter.h"
#include "core/timeline.h"
#include "core/match_mediator.h"
#include "core/rating_map.h"
#include "core/user.h"
#include "exception.h"
#include "misc/prng.h"

#include <future>
#include <type_traits>

namespace matchmaker::core {

class MatchEngine {
public:
    explicit MatchEngine(misc::PRNG& prng, Waiter& waiter);

    MatchEngine(const MatchEngine&) = delete;
    MatchEngine& operator=(const MatchEngine&) = delete;

    MatchEngine(MatchEngine&&) = delete;
    MatchEngine& operator=(MatchEngine&&) = delete;

    RatingMap run(RatingMap&& rating_map);

    void keep_alive();
    void let_die();

    template<typename F,
        typename R = std::invoke_result_t<F, RatingMap&>,
        std::enable_if_t<std::is_invocable_r_v<R, F, RatingMap&>, int> = 0>
    R intercept(F f)
    {
        std::promise<R> promise;
        auto future = promise.get_future();
        timeline.post([this, f, &promise]
            {
                try {
                    if constexpr (std::is_void_v<R>) {
                        f(rating_map);
                        promise.set_value();
                    } else {
                        promise.set_value(f(rating_map));
                    }
                } catch (...) {
                    promise.set_exception(std::current_exception());
                }
            });
        return future.get();
    }

    template<typename F = void(*)(), std::enable_if_t<std::is_invocable_v<F, const User&>, int> = 0>
    void add_user(const User& user, F on_add = [](auto&&){})
    {
        timeline.post([this, &user, on_add]{ new_player_for(user).init(); on_add(user); });
    }

    template<typename F = void(*)(), std::enable_if_t<std::is_invocable_v<F, const User&>, int> = 0>
    void rem_user(const User& user, F on_rem = [](auto&&){})
    {
        timeline.post([this, &user, on_rem]{ del_player_of(user)->deinit(); on_rem(user); });
    }

    template<typename F = void(*)(), std::enable_if_t<std::is_invocable_v<F>, int> = 0>
    void rem_all_users(F on_rem = []{})
    {
        timeline.post([this, on_rem]{ del_all_players(); on_rem(); });
    }

    inline void set_player_observer(PlayerObserver *player_observer)
    {
        this->player_observer = player_observer;
    }

private:
    Player& new_player_for(const User& user);
    std::unique_ptr<Player> del_player_of(const User& user);
    void del_all_players();

    misc::PRNG& prng;

    Timeline timeline;
    EventHandle sentinel_event;

    PlayerObserver *player_observer = nullptr;

    RatingMap rating_map;
    MatchMediator mediator {rating_map, timeline};
    std::unordered_map<const User *, std::unique_ptr<Player>> player_by_user;
};

class MatchEngineException : public Exception {
public:
    using Exception::Exception;
};

}
