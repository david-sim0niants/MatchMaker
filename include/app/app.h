#pragma once

#include "core/main_activity.h"

#include <QApplication>

namespace matchmaker::app {

class App {
public:
    App(int argc, char *argv[]);

    int exec();

private:
    int argc;
    char **argv;
    core::MainActivity main_activity;
};

}
