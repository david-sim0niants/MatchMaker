#include "gui/user_list.h"
#include "gui/user_list_model.h"
#include "gui/user_list_delegate.h"
#include "gui/user_list_filter.h"

#include <QWidget>
#include <QBoxLayout>
#include <QHeaderView>

namespace matchmaker::gui {

UserList::UserList(QWidget *parent) :
    QWidget(parent),
    table_view(new QTableView(this)),
    delegate(new UserListDelegate(this)),
    user_list_filter(new UserListFilter(this))
{
    init();
}

void UserList::init()
{
    init_table_view();
    init_layout();
    connect(delegate, &UserListDelegate::clicked_add_user, this, &UserList::clicked_add_user);
    connect(user_list_filter, &UserListFilter::trigerred, this, &UserList::on_filter_users);
}

void UserList::on_filter_users(QStringView pattern)
{
    qDebug() << "\nFilter by " << pattern << '\n';
}

void UserList::init_table_view()
{
    table_view->setModel(new UserListModel(this));
    table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_view->setSelectionMode(QAbstractItemView::NoSelection);
    table_view->setItemDelegate(delegate);
    table_view->show();
}

void UserList::init_layout()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(table_view);
    layout->addWidget(user_list_filter);
}

}
