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
    struct Context {
        RatingMap rating_map;
    };

public:
    explicit MatchEngine(misc::PRNG& prng, Waiter& waiter);

    MatchEngine(const MatchEngine&) = delete;
    MatchEngine& operator=(const MatchEngine&) = delete;

    MatchEngine(MatchEngine&&) = delete;
    MatchEngine& operator=(MatchEngine&&) = delete;

    RatingMap run(RatingMap&& rating_map);

    void keep_alive();
    void let_die();

    template<typename F = void (*)(Context&, const User&)>
    void add_user(const User *user, F on_add = [](auto&&...){})
    {
        timeline.post([this, user, on_add]
            {
                new_player_for(user).init();
                on_add(context, user);
            });
    }

    template<typename F = void (*)(Context&, const User&)>
    void rem_user(const User *user, F on_rem = [](auto&&...){})
    {
        timeline.post([this, user, on_rem]
            {
                del_player_of(user)->deinit();
                on_rem(context, user);
            });
    }

    template<typename F = void (*)(Context&)>
    void rem_all_users(F on_rem = [](Context&){})
    {
        timeline.post([this, on_rem]{ del_all_players(); on_rem(context); });
    }

    inline void set_player_observer(PlayerObserver *player_observer)
    {
        this->player_observer = player_observer;
    }

    template<typename F = void(*)(Context&)>
    void intercept_async(F f)
    {
        timeline.post([this, f] { f(context); });
    }

    template<typename F = void(*)(Context&), typename R = std::invoke_result_t<F, Context&>>
    R intercept(F f)
    {
        std::promise<R> promise;
        auto future = promise.get_future();
        timeline.post([this, &f, &promise]
            {
                try {
                    if constexpr (std::is_void_v<R>) {
                        f(context);
                        promise.set_value();
                    } else {
                        promise.set_value(f(context));
                    }
                } catch (...) {
                    promise.set_exception(std::current_exception());
                }
            });
        return future.get();
    }

private:
    Player& new_player_for(const User *user);
    std::unique_ptr<Player> del_player_of(const User *user);
    void del_all_players();

    misc::PRNG& prng;

    Timeline timeline;
    EventHandle sentinel_event;

    PlayerObserver *player_observer = nullptr;

    Context context;
    MatchMediator mediator {context.rating_map, timeline};
    std::unordered_map<const User *, std::unique_ptr<Player>> player_by_user;
};

using MatchEngineContext = MatchEngine::Context;

class MatchEngineException : public Exception {
public:
    using Exception::Exception;
};

}
