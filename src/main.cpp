#include <QApplication>
#include <future>

#include "core/cv_waiter.h"
#include "core/match_mediator.h"
#include "core/player.h"
#include "core/rating_map.h"
#include "core/user.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    using namespace matchmaker;

    core::User user("johndoe", "John", "Doe", {nullptr});
    core::CVWaiter cv_waiter;
    core::Timeline timeline(cv_waiter);
    core::RatingMap rating_map;
    core::MatchMediator mediator(rating_map, timeline);
    core::Player player {user, mediator};

    timeline.join([&]{ player.init(); });

    auto fut_timeline_done = std::async([&timeline]{ timeline.run(); });
    int ret_code = app.exec();
    fut_timeline_done.get();
    return ret_code;
}
