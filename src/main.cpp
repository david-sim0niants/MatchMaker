#include "core/match_engine.h"
#include "core/cv_waiter.h"
#include "core/game_factory.h"
#include "misc/printing.h"
#include "gui/mainwindow.h"

#include <QApplication>
#include <filesystem>
#include <iostream>

using namespace matchmaker;
using namespace matchmaker::core;

class ConsoleUserRatingObserver : public UserRatingObserver {
public:
    void notify_rating_change(const Game& game, const User& user, int rating)
    {
        misc::print_to(std::cout,
                user.get_username(), "'s rating for ", game.get_name(),
                " changed to ", rating);
        std::cout << std::endl;
    }
};

class ConsolePlayerObserver : public PlayerObserver {
public:
    void on_state_change(const Player& player, Player::State state, Time time)
    {
        misc::print_to(std::cout, player.get_user().get_username(),
                " changed state to: ", state, " at time: ", time);
        std::cout << std::endl;
    }

    void on_match_start(const Player& player, Match& match, Time time)
    {
        misc::print_to(std::cout, player.get_user().get_username(),
                " started a match playing: ", match.get_game().get_name(),
                " at time: ", time);
        std::cout << std::endl;
    }

    void on_match_leave(const Player& player, Time time)
    {
        misc::print_to(std::cout, player.get_user().get_username(),
                " left a match at time: ", time);
        std::cout << std::endl;
    }
};

static const char *const available_games[] = {"tic-tac-toe-3x3", "tic-tac-toe-4x4", "tic-tac-toe-5x5", "rock-paper-scissors"};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    gui::MainWindow w;
    w.show();

    std::filesystem::path exe_path(argv[0]);
    ExecutableGameFactory exe_game_factory {exe_path.parent_path()};

    std::optional<ExecutableGame> exe_games[std::size(available_games)];
    for (int i = 0; i < std::size(available_games); ++i) {
        exe_games[i] = exe_game_factory.create(available_games[i]);
        if (! exe_games[i].has_value()) {
            std::cerr << "Error: could not load " << available_games[i] << std::endl;
            return EXIT_FAILURE;
        }
    }

    User johndoe("johndoe", "John", "Doe", {&*exe_games[0], &*exe_games[1], &*exe_games[3]});
    User alice("alice", "Alice", "Cooper", {&*exe_games[0], &*exe_games[2], &*exe_games[3]});
    User bob("bob", "Bob", "Dylan", {&*exe_games[1], &*exe_games[2], &*exe_games[3]});

    CVWaiter waiter;
    misc::PRNG prng;

    ConsoleUserRatingObserver user_rating_observer;
    ConsolePlayerObserver player_observer;

    MatchEngine match_engine(prng, waiter, &user_rating_observer, &player_observer);

    match_engine.keep_alive();

    auto fut_done = match_engine.run_async();
    match_engine.add_user(johndoe);
    match_engine.add_user(alice);
    match_engine.add_user(bob);

    match_engine.let_die();

    int ret_code = app.exec();

    match_engine.rem_all_users();

    fut_done.get();
    return ret_code;
}
