#pragma once

#include "core/rating_map.h"

namespace matchmaker::app {

class ConsoleUserRatingObserver : public core::RatingMapObserver {
public:
    void notify_rating_change(const core::Game& game, const core::User& user, int rating)
        override;
};

}
