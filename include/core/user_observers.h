#pragma once

#include "core/rating_map.h"
#include "core/user.h"

namespace matchmaker::core {

class UserRatingObserver {
public:
    virtual void notify_rating_change(const Game& game, const User& user, Rating rating) = 0;
};

class RatingMapToUserRatingObserverAdapter : public RatingMapObserver {
public:
    explicit RatingMapToUserRatingObserverAdapter(
            UserRatingObserver *user_rating_observer = nullptr);

    void notify_rating_change(const Game& game, const Player& player, Rating rating) override;

private:
    UserRatingObserver *user_rating_observer;
};

}
