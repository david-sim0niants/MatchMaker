#pragma once

#include <QApplication>

namespace matchmaker::app {

class App {
public:
    App(int argc, char *argv[]);

    int exec();

private:
    int argc;
    char **argv;
};

}
