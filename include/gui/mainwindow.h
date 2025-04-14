#pragma once

#include "gui/menubar.h"
#include "gui/dashboard.h"
#include "gui/user_list.h"

#include <QMainWindow>

namespace matchmaker::gui {

class MainWindow : public QMainWindow {
    Q_OBJECT;
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    MenuBar *menu_bar;
    Dashboard *dashboard;
    UserList *user_list;

private slots:
    void toggle_dashboard();
    void toggle_user_list();
};

}
