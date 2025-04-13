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
}

}
