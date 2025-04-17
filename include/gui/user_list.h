#pragma once

#include "gui/user_descriptor.h"
#include "gui/user_list_model.h"
#include "gui/user_list_delegate.h"
#include "gui/user_list_filter.h"

#include <QWidget>
#include <QTableView>
#include <QSortFilterProxyModel>

namespace matchmaker::gui {

class UserListEndpoint;

class UserList : public QWidget {
    Q_OBJECT;
public:
    explicit UserList(UserListEndpoint& endpoint, QWidget *parent = nullptr);

signals:
    void clicked_add_user();

public slots:
    void on_added_user(UserDescriptor user);

private slots:
    void on_filter_users(const QString& pattern);

private:
    void init();
    void init_table_view();
    void init_filter();
    void init_layout();

    UserListEndpoint& endpoint;
    UserListModel *model;
    QSortFilterProxyModel *model_proxy;
    UserListDelegate *delegate;
    UserListFilter *user_list_filter;
    QTableView *table_view;
};

class UserListEndpoint {
public:
    virtual QList<UserDescriptor> get_initial_users() const = 0;
};

}
