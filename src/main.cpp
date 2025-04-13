#include "core/cv_waiter.h"
#include "core/match_engine.h"
#include "core/user.h"
#include "gui/mainwindow.h"

#include <QApplication>
#include <iostream>

using namespace matchmaker;

class UserRatingObserver : core::UserRatingObserver {
public:
    void notify_rating_change(const core::Game& game, const core::User& user, int rating)
    {
        std::cout << game.get_name() << " " << user.get_name() << " " << rating << std::endl;
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    gui::MainWindow w;
    w.show();

    core::CVWaiter waiter;
    core::User johndoe("johndoe", "John", "Doe", {nullptr});
    core::User alice("alice", "Alice", "Cooper", {nullptr});

    core::MatchEngine match_engine(waiter);

    match_engine.add_user(johndoe);
    match_engine.add_user(alice);
    auto fut_done = match_engine.run_async();

    int ret_code = app.exec();
    fut_done.get();
    return ret_code;
}
