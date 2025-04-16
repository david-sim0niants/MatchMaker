#pragma once

#include "core/game_registry.h"
#include "core/user_registry.h"

#include <QApplication>

namespace matchmaker::app {

class App {
public:
    App(int argc, char *argv[]);

    int exec();

private:
    int argc;
    char **argv;

    core::GameRegistry game_registry;
    core::UserRegistry user_registry;
};

}
