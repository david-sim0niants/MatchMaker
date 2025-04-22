#include "app/console_player_observer.h"
#include "core/match.h"
#include "misc/printing.h"

#include <iostream>

namespace matchmaker::app {

using namespace core;

void ConsolePlayerObserver::on_state_change(
        const Player& player, Player::State state, Time time)
{
    misc::print_to(std::cout, player.get_user()->get_username(),
            " changed state to: ", state, " at time: ", time);
    std::cout << std::endl;
}

void ConsolePlayerObserver::on_match_start(const Player& player, const Match& match, Time time)
{
    misc::print_to(std::cout, player.get_user()->get_username(),
            " started a match playing: ", match.get_game()->get_name(),
            " at time: ", time);
    std::cout << std::endl;
}

void ConsolePlayerObserver::on_match_leave(const Player& player, Time time)
{
    misc::print_to(std::cout, player.get_user()->get_username(),
            " left a match at time: ", time);
    std::cout << std::endl;
}

}
