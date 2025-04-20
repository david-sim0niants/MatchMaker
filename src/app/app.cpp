#include "app/app.h"
#include "app/mediator.h"
#include "app/games.h"
#include "app/console_player_observer.h"
#include "app/console_user_rating_observer.h"

#include "core/main_activity.h"
#include "gui/mainwindow.h"

namespace matchmaker::app {

App::App(int argc, char *argv[]) :
    argc(argc), argv(argv),
    main_activity(core::GameRegistry(argv[0], game_infos, std::size(game_infos)), "/tmp/")
{
}

int App::exec()
{
    ConsolePlayerObserver player_observer;
    ConsoleUserRatingObserver rating_observer;

    main_activity.set_player_observer(&player_observer);
    main_activity.set_rating_map_observer(&rating_observer);

    Mediator mediator {main_activity};

    QApplication q_app(argc, argv);
    gui::MainWindow main_window {mediator};

    main_window.show();
    return q_app.exec();
}

}
