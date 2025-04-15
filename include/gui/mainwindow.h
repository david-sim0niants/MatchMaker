#pragma once

#include "gui/add_user_dialog.h"
#include "gui/menubar.h"
#include "gui/dashboard.h"
#include "gui/user_list.h"

#include <QMainWindow>

namespace matchmaker::gui {

class MainWindowEndpoint;

class MainWindow : public QMainWindow {
    Q_OBJECT;
public:
    explicit MainWindow(MainWindowEndpoint& endpoint, QWidget *parent = nullptr);

private:
    MenuBar *menu_bar;
    Dashboard *dashboard;
    UserList *user_list;
    AddUserDialog *add_user_dialog;

private slots:
    void toggle_dashboard();
    void toggle_user_list();
};

class MainWindowEndpoint : public AddUserDialogEndpoint {
};

}
