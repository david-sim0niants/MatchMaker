#pragma once

#include "core/user.h"

namespace matchmaker::core {

class UserRatingObserver {
public:
    virtual void notify_rating_change(const Game& game, const User& user, int rating) = 0;
};

}
