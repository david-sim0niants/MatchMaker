#include "gui/mainwindow.h"
#include "gui/user_list.h"

#include <QWidget>
#include <QHBoxLayout>

namespace matchmaker::gui {

MainWindow::MainWindow(MainWindowEndpoint& endpoint, QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Match Maker");
    setCentralWidget(new QWidget(this));

    menu_bar = new MenuBar(centralWidget());
    setMenuBar(menu_bar);

    QHBoxLayout *horizontal_layout = new QHBoxLayout(centralWidget());
    centralWidget()->setLayout(horizontal_layout);

    dashboard = new Dashboard(centralWidget());
    user_list = new UserList(endpoint, centralWidget());
    add_user_dialog = new AddUserDialog(endpoint, centralWidget());

    horizontal_layout->addWidget(dashboard);
    horizontal_layout->addWidget(user_list);

    connect(menu_bar, &MenuBar::clicked_show_hide_dashboard,
            this, &MainWindow::toggle_dashboard);
    connect(menu_bar, &MenuBar::clicked_show_hide_user_list,
            this, &MainWindow::toggle_user_list);

    connect(menu_bar, &MenuBar::clicked_add_user,
            [this]{ add_user_dialog->exec(); });
    connect(user_list, &UserList::clicked_add_user,
            [this]{ add_user_dialog->exec(); });

    connect(menu_bar, &MenuBar::clicked_rem_user,
            user_list, &UserList::on_rem_selected_user);

    connect(add_user_dialog, &AddUserDialog::added_user,
            user_list, &UserList::on_added_user);

    dashboard->show();
    user_list->show();

    resize(1200, 800);
}

void MainWindow::toggle_dashboard()
{
    dashboard->setVisible(! dashboard->isVisible());
}

void MainWindow::toggle_user_list()
{
    user_list->setVisible(! user_list->isVisible());
}

}
