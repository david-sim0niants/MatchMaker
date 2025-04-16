#pragma once

#include "core/user_observers.h"

namespace matchmaker::app {

class ConsoleUserRatingObserver : public core::UserRatingObserver {
public:
    void notify_rating_change(const core::Game& game, const core::User& user, int rating)
        override;
};

}
