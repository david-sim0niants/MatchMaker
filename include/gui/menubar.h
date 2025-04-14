#pragma once

#include <QMenuBar>

namespace matchmaker::gui {

class MenuBar : public QMenuBar {
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = nullptr);

signals:
    void clicked_save_dashboard_file() const;
    void clicked_add_user() const;
    void clicked_del_user() const;
    void clicked_show_hide_dashboard() const;
    void clicked_show_hide_user_list() const;
};

}
