#pragma once

#include "core/waiter.h"
#include "core/timeline.h"
#include "core/match_mediator.h"
#include "core/rating_map.h"
#include "core/user.h"
#include "core/user_rating_observer.h"
#include "exception.h"

#include <future>
#include <type_traits>

namespace matchmaker::core {

class MatchEngine {
private:
    class RatingMapObserverAdapter : public RatingMapObserver {
    public:
        RatingMapObserverAdapter(UserRatingObserver *user_rating_observer);
        void notify_rating_change(const Game& game, const Player& player, Rating rating);

    private:
        UserRatingObserver *user_rating_observer;
    };

public:
    explicit MatchEngine(Waiter& waiter, UserRatingObserver *rating_observer = nullptr);

    MatchEngine(const MatchEngine&) = delete;
    MatchEngine& operator=(const MatchEngine&) = delete;

    MatchEngine(MatchEngine&&) = delete;
    MatchEngine& operator=(MatchEngine&&) = delete;

    void run();

    inline std::future<void> run_async()
    {
        return std::async(&MatchEngine::run, this);
    }

    template<typename F = void(*)(), std::enable_if_t<std::is_invocable_v<F>, int> = 0>
    void add_user(const User& user, F on_add = []{})
    {
        timeline.join([this, &user, on_add]{ new_player_for(user)->init(); on_add(); });
    }

    template<typename F = void(*)(), std::enable_if_t<std::is_invocable_v<F>, int> = 0>
    void rem_user(const User& user, F on_rem = []{})
    {
        timeline.join([this, &user, on_rem]{ del_player_of(user)->deinit(); on_rem(); });
    }

private:
    std::shared_ptr<Player> new_player_for(const User& user);
    std::shared_ptr<Player> del_player_of(const User& user);

    Waiter& waiter;
    RatingMapObserverAdapter rating_map_observer_adapter;
    RatingMap rating_map;
    Timeline timeline {waiter};
    MatchMediator mediator {rating_map, timeline};
    std::unordered_map<const User *, std::shared_ptr<Player>> player_by_user;
};

class MatchEngineException : public Exception {
public:
    using Exception::Exception;
};

}
