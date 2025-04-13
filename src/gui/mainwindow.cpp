#include "gui/mainwindow.h"

#include <QWidget>
#include <QHBoxLayout>

namespace matchmaker::gui {

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Match Maker");
    central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    menu_bar = new MenuBar(central_widget);
    setMenuBar(menu_bar);

    QHBoxLayout *horizontal_layout = new QHBoxLayout(central_widget);
    central_widget->setLayout(horizontal_layout);

    user_list = new QWidget(central_widget);
    dashboard = new QWidget(central_widget);

    dashboard->setStyleSheet("background-color: lightblue;");
    user_list->setStyleSheet("background-color: green;");

    horizontal_layout->addWidget(dashboard);
    horizontal_layout->addWidget(user_list);

    resize(800, 600);
}

}
