#include "gui/mainwindow.h"

#include <QWidget>
#include <QHBoxLayout>

namespace matchmaker::gui {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Match Maker");
    setCentralWidget(new QWidget(this));

    menu_bar = new MenuBar(centralWidget());
    setMenuBar(menu_bar);

    QHBoxLayout *horizontal_layout = new QHBoxLayout(centralWidget());
    centralWidget()->setLayout(horizontal_layout);

    dashboard = new Dashboard(centralWidget());
    user_list = new UserList(centralWidget());

    // dashboard->setStyleSheet("background-color: lightblue;");
    // user_list->setStyleSheet("background-color: green;");

    horizontal_layout->addWidget(dashboard);
    horizontal_layout->addWidget(user_list);

    connect(menu_bar, &MenuBar::show_hide_dashboard, this, &MainWindow::toggle_dashboard);
    connect(menu_bar, &MenuBar::show_hide_user_list, this, &MainWindow::toggle_user_list);

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
