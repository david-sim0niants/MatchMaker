#include "gui/menubar.h"

namespace matchmaker::gui {

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    QMenu *file_menu = addMenu("File");
    QAction *save_dashboard_file = file_menu->addAction("Save the Dashboard to File");

    QMenu *edit_menu = addMenu("Edit");
    QAction *add_user = edit_menu->addAction("Add User");
    QAction *del_user = edit_menu->addAction("Remove User");

    QMenu *view_menu = addMenu("View");
    QAction *show_hide_dashboard = view_menu->addAction("Show/Hide Dashboard");
    QAction *show_hide_user_list = view_menu->addAction("Show/Hide User List");

    connect(save_dashboard_file, &QAction::triggered,
            this, &MenuBar::clicked_save_dashboard_file);
    connect(add_user, &QAction::triggered,
            this, &MenuBar::clicked_add_user);
    connect(del_user, &QAction::triggered,
            this, &MenuBar::clicked_del_user);
    connect(show_hide_dashboard, &QAction::triggered,
            this, &MenuBar::clicked_show_hide_dashboard);
    connect(show_hide_user_list, &QAction::triggered, this,
            &MenuBar::clicked_show_hide_user_list);
}

}
