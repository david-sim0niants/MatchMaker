#include "app/app.h"
#include "app/mediator.h"
#include "app/games.h"
#include "app/console_player_observer.h"
#include "app/console_user_rating_observer.h"

#include "core/main_activity.h"
#include "gui/main_window.h"

#include <cstdlib>
#include <iostream>
#include <filesystem>

namespace matchmaker::app {

namespace fs = std::filesystem;

static std::optional<int> get_prng_seed()
{
    const char *seed = getenv("SEED");
    try {
        if (seed)
            return std::atoi(seed);
    } catch (...) {}
    return std::nullopt;
}

App::App(int argc, char *argv[]) :
    argc(argc), argv(argv)
{
}

int App::exec()
{
    std::string_view games_path = argv[0];

    fs::path data_path = fs::path(argc > 1 ? argv[1] : ".") / "matchmaker_data";
    if (! fs::exists(data_path) && ! fs::create_directory(data_path)) {
        std::cerr << "Failed to create a data directory";
        return EXIT_FAILURE;
    }

    std::string data_path_str = data_path.string();

    auto prng_seed = get_prng_seed();
    misc::PRNG prng = prng_seed ? misc::PRNG(*prng_seed) : misc::PRNG();

    core::MainActivity main_activity {
        prng, game_infos, std::size(game_infos), games_path, data_path_str };

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
