#pragma once

#include "core/waiter.h"
#include "core/timeline.h"
#include "core/match_mediator.h"
#include "core/rating_map.h"
#include "core/user.h"
#include "core/user_rating_observer.h"
#include "exception.h"

#include <future>

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

    void add_user(const User& user);

    void rem_user(const User& user);

private:
    Player& new_player_for(const User& user);
    Player del_player_of(const User& user);

    Waiter& waiter;
    RatingMapObserverAdapter rating_map_observer_adapter;
    RatingMap rating_map;
    Timeline timeline {waiter};
    MatchMediator mediator {rating_map, timeline};
    std::unordered_map<const User *, Player *> player_by_user;
};

class MatchEngineException : public Exception {
public:
    using Exception::Exception;
};

}
