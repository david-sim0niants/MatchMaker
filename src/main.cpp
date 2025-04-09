#include <QApplication>
#include <future>

#include "core/player.h"
#include "core/waiter.h"
#include "core/user.h"
#include "misc/prng.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    matchmaker::core::User user("johndoe", "John", "Doe", {nullptr});
    matchmaker::core::SleepingWaiter sleeping_waiter;
    matchmaker::core::Timeline timeline(sleeping_waiter);
    matchmaker::misc::PRNG prng;
    matchmaker::core::Player player(user, timeline, prng);

    auto fut_timeline_done = std::async([&timeline]{ timeline.run(); });
    int ret_code = app.exec();
    fut_timeline_done.wait();
    return ret_code;
}
