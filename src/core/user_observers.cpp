#include "core/user_observers.h"
#include "core/player.h"

namespace matchmaker::core {

RatingMapToUserRatingObserverAdapter::
    RatingMapToUserRatingObserverAdapter(
            UserRatingObserver *user_rating_observer) :
        user_rating_observer(user_rating_observer)
{
}

void RatingMapToUserRatingObserverAdapter::notify_rating_change(
        const Game& game, const Player& player, Rating rating)
{
    user_rating_observer->notify_rating_change(game, player.get_user(), rating);
}

}
