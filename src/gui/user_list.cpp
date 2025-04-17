#include "gui/user_list.h"

#include <QWidget>
#include <QBoxLayout>
#include <QHeaderView>

namespace matchmaker::gui {

UserList::UserList(UserListEndpoint& endpoint, QWidget *parent) :
    QWidget(parent),
    endpoint(endpoint),
    model(new UserListModel(endpoint.get_initial_users(), Qt::AscendingOrder, this)),
    delegate(new UserListDelegate(this)),
    user_list_filter(new UserListFilter(this)),
    table_view(new QTableView(this))
{
    init();
}

void UserList::on_added_user(UserDescriptor user)
{
    model->add_user(user);
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
    table_view->setModel(model);
    table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_view->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    table_view->setItemDelegate(delegate);
    table_view->setSortingEnabled(true);
    table_view->sortByColumn(int(UserProperty::Username), Qt::AscendingOrder);
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
