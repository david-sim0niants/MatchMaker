#pragma once

#include "gui/add_user_dialog.h"
#include "gui/menubar.h"
#include "gui/dashboard.h"
#include "gui/user_list.h"

#include <QMainWindow>

namespace matchmaker::gui {

class MainWindowEndpoint;

class MainWindowControl {
public:
    virtual DashboardControl& get_dashboard_control() = 0;
    virtual UserListControl& get_user_list_control() = 0;

    virtual void add_user(UserDescriptor user) = 0;
};

class MainWindow : public QMainWindow, private MainWindowControl {
    Q_OBJECT;
public:
    explicit MainWindow(MainWindowEndpoint& endpoint, QWidget *parent = nullptr);

private:
    DashboardControl& get_dashboard_control() override;
    UserListControl& get_user_list_control() override;

    void add_user(UserDescriptor user) override;

signals:
    void added_user(UserDescriptor user);

private slots:
    void on_toggle_dashboard();
    void on_toggle_user_list();

    void on_rem_user(UserDescriptor user);

    void on_save_dashboard_file();
    void on_save_ratings_for_game(const QString& game);

private:
    void closeEvent(QCloseEvent *event) override;

    void start();
    void stop();

    MainWindowEndpoint& endpoint;
    MenuBar *menu_bar;
    Dashboard *dashboard;
    UserList *user_list;
    AddUserDialog *add_user_dialog;
};

class MainWindowEndpoint {
public:
    virtual void start(MainWindowControl& control) = 0;
    virtual void stop() = 0;

    virtual void save_game_ratings_to_file(const QString& game) = 0;
    virtual void save_all_ratings_to_file() = 0;

    virtual void rem_user(UserDescriptor user) = 0;
    virtual AddUserDialogEndpoint& get_add_user_dialog_endpoint() = 0;
};

}
