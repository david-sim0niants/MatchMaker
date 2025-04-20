#pragma once

#include "gui/user_descriptor.h"
#include "gui/user_list_model.h"
#include "gui/user_list_delegate.h"
#include "gui/user_list_filter.h"

#include <QWidget>
#include <QTableView>
#include <QSortFilterProxyModel>

namespace matchmaker::gui {

class UserListControl {
public:
    virtual void set_initial_users(const QList<UserDescriptor>& users) = 0;
};

class UserList : public QWidget, public UserListControl {
    Q_OBJECT;
public:
    explicit UserList(QWidget *parent = nullptr);

    void set_initial_users(const QList<UserDescriptor>& users) override;

signals:
    void clicked_add_user();
    void removed_user(UserDescriptor user);

public slots:
    void on_added_user(UserDescriptor user);
    void on_rem_selected_user();

private:
signals:
    void init_users(const QList<UserDescriptor>& users);

private slots:
    void on_filter_users(const QString& pattern);
    void on_open_context_menu(const QPoint& pos);

private:
    void init();
    void init_table_view();
    void init_filter();
    void init_layout();

    bool has_selection() const;
    std::pair<int, int> get_selected_rows() const;

    UserListModel *model;
    QSortFilterProxyModel *model_proxy;
    UserListDelegate *delegate;
    UserListFilter *user_list_filter;
    QTableView *table_view;
};

}
