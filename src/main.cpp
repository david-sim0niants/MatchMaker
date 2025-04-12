#include <QApplication>
#include <future>

#include "core/cv_waiter.h"
#include "core/match_engine.h"
#include "core/user.h"
#include "mainwindow.h"

using namespace matchmaker;

class UserRatingObserver : core::UserRatingObserver {
public:
    void notify_rating_change(const core::Game& game, const core::User& user, int rating)
    {
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    core::CVWaiter waiter;
    core::User johndoe("johndoe", "John", "Doe", {nullptr});
    core::User alice("alice", "Alice", "Cooper", {nullptr});

    core::MatchEngine match_engine(waiter);

    match_engine.add_user(johndoe);
    match_engine.add_user(alice);
    match_engine.run();

    // int ret_code = app.exec();
    // return ret_code;
    return 0;
}
