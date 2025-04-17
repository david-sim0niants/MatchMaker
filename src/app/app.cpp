#include "app/app.h"
#include "app/mediator.h"
#include "app/games.h"
#include "app/console_player_observer.h"
#include "app/console_user_rating_observer.h"

#include "core/cv_waiter.h"
#include "core/match_engine.h"
#include "gui/mainwindow.h"

namespace matchmaker::app {

App::App(int argc, char *argv[]) :
    argc(argc), argv(argv),
    game_registry(argv[0], game_infos, std::size(game_infos))
{
}

int App::exec()
{
    misc::PRNG prng;
    core::CVWaiter cv_waiter;

    ConsoleUserRatingObserver user_rating_observer;
    ConsolePlayerObserver player_observer;

    core::MatchEngine match_engine {prng, cv_waiter, &user_rating_observer, &player_observer};

    Mediator mediator {game_registry, user_registry, match_engine};

    QApplication q_app(argc, argv);
    gui::MainWindow main_window {mediator};

    match_engine.keep_alive();
    auto fut_done = match_engine.run_async();

    main_window.show();
    int status = q_app.exec();

    match_engine.let_die();
    match_engine.rem_all_users();

    fut_done.get();
    return status;
}

}
