#pragma once

#include <QMenuBar>

namespace matchmaker::gui {

class MenuBar : public QMenuBar {
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = nullptr);

signals:
    void save_dashboard_file();
    void add_user();
    void del_user();
    void show_hide_dashboard();
    void show_hide_user_list();

private slots:
    void on_show_hide_dashboard()
    {
        emit show_hide_dashboard();
    }

    void on_show_hide_user_list()
    {
        emit show_hide_user_list();
    }
};

}
