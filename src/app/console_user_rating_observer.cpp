#include "app/console_user_rating_observer.h"
#include "misc/printing.h"

#include <iostream>

namespace matchmaker::app {

void ConsoleUserRatingObserver::notify_rating_change(
        const core::Game& game, const core::User& user, int rating)
{
    misc::print_to(std::cout,
            user.get_username(), "'s rating for ", game.get_name(),
            " changed to ", rating);
    std::cout << std::endl;
}

}
