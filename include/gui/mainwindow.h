#pragma once

#include "gui/menubar.h"

#include <QMainWindow>

namespace matchmaker::gui {

class MainWindow : public QMainWindow {
    Q_OBJECT;
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QWidget *central_widget;
    MenuBar *menu_bar;
    QWidget *user_list;
    QWidget *dashboard;

private slots:
    void toggle_dashboard();
    void toggle_user_list();
};

}
