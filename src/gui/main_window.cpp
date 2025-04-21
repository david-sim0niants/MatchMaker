#include "gui/main_window.h"

#include <QWidget>
#include <QHBoxLayout>

namespace matchmaker::gui {

MainWindow::MainWindow(MainWindowEndpoint& endpoint, QWidget *parent) :
    QMainWindow(parent), endpoint(endpoint)
{
    setWindowTitle("Match Maker");
    setCentralWidget(new QWidget(this));

    menu_bar = new MenuBar(centralWidget());
    setMenuBar(menu_bar);

    QHBoxLayout *horizontal_layout = new QHBoxLayout(centralWidget());
    centralWidget()->setLayout(horizontal_layout);

    dashboard = new Dashboard(centralWidget());
    user_list = new UserList(centralWidget());
    add_user_dialog = new AddUserDialog(endpoint.get_add_user_dialog_endpoint(), centralWidget());

    horizontal_layout->addWidget(dashboard);
    horizontal_layout->addWidget(user_list);

    connect(menu_bar, &MenuBar::clicked_show_hide_dashboard,
            this, &MainWindow::on_toggle_dashboard);
    connect(menu_bar, &MenuBar::clicked_show_hide_user_list,
            this, &MainWindow::on_toggle_user_list);

    connect(menu_bar, &MenuBar::clicked_add_user,
            [this]{ add_user_dialog->exec(); });
    connect(user_list, &UserList::clicked_add_user,
            [this]{ add_user_dialog->exec(); });

    connect(menu_bar, &MenuBar::clicked_rem_user,
            user_list, &UserList::on_rem_selected_user);

    connect(this, &MainWindow::added_user, user_list, &UserList::on_added_user);
    connect(this, &MainWindow::added_user, dashboard, &Dashboard::on_add_user);

    connect(user_list, &UserList::removed_user, this, &MainWindow::on_rem_user);

    connect(menu_bar, &MenuBar::clicked_save_dashboard_file,
            this, &MainWindow::on_save_dashboard_file);

    connect(dashboard, &Dashboard::clicked_save_game_ratings,
            this, &MainWindow::on_save_ratings_for_game);

    dashboard->show();
    user_list->show();

    resize(1200, 800);

    start();
}

DashboardControl& MainWindow::get_dashboard_control()
{
    return *dashboard;
}

UserListControl& MainWindow::get_user_list_control()
{
    return *user_list;
}

void MainWindow::add_user(UserDescriptor user)
{
    emit added_user(user);
}

void MainWindow::on_toggle_dashboard()
{
    dashboard->setVisible(! dashboard->isVisible());
}

void MainWindow::on_toggle_user_list()
{
    user_list->setVisible(! user_list->isVisible());
}

void MainWindow::on_rem_user(UserDescriptor user)
{
    dashboard->on_rem_user(user);
    endpoint.rem_user(user);
}

void MainWindow::on_save_dashboard_file()
{
    endpoint.save_all_ratings_to_file();
}

void MainWindow::on_save_ratings_for_game(const QString& game)
{
    endpoint.save_game_ratings_to_file(game);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    stop();
}

void MainWindow::start()
{
    endpoint.start(*this);
}

void MainWindow::stop()
{
    endpoint.stop();
}

}
