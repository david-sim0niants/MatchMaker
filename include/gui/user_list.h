#pragma once

#include "gui/user_list_filter.h"

#include <QTableView>
#include <QWidget>

namespace matchmaker::gui {

class UserListDelegate;

class UserList : public QWidget {
    Q_OBJECT;
public:
    explicit UserList(QWidget *parent = nullptr);

private slots:
    void on_click_add_user();
    void on_filter_users(QStringView pattern);

private:
    void init();
    void init_table_view();
    void init_filter();
    void init_layout();

    QTableView *table_view;
    UserListDelegate *delegate;
    UserListFilter *user_list_filter;
};

}
