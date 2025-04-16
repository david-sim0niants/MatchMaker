#pragma once

#include "gui/user_descriptor.h"
#include "gui/user_list_model.h"
#include "gui/user_list_delegate.h"
#include "gui/user_list_filter.h"

#include <QTableView>
#include <QWidget>

namespace matchmaker::gui {

class UserList : public QWidget {
    Q_OBJECT;
public:
    explicit UserList(QWidget *parent = nullptr);

signals:
    void clicked_add_user();

public slots:
    void on_added_user(UserDescriptor user);

private slots:
    void on_filter_users(QStringView pattern);

private:
    void init();
    void init_table_view();
    void init_filter();
    void init_layout();

    UserListModel *model;
    UserListDelegate *delegate;
    UserListFilter *user_list_filter;
    QTableView *table_view;
};

}
