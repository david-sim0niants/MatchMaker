#pragma once

#include <QMenuBar>

namespace matchmaker::gui {

class MenuBar : public QMenuBar {
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = nullptr);

signals:
    void save_dashboard_file() const;
    void add_user() const;
    void del_user() const;
    void show_hide_dashboard() const;
    void show_hide_user_list() const;
};

}
