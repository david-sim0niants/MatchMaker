#include "gui/user_list.h"

#include <QWidget>
#include <QBoxLayout>
#include <QHeaderView>
#include <QRegularExpression>

namespace matchmaker::gui {

namespace {

/* Custom SortFilterProxyModel to ingore the last row. */
class SortFilterProxyModel: public QSortFilterProxyModel
{
public:
    using QSortFilterProxyModel::QSortFilterProxyModel;

protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override
    {
        if (std::max(left.row(), right.row()) >= rowCount() - 1) {
            if (sortOrder() == Qt::AscendingOrder)
                return left.row() < right.row();
            else
                return left.row() > right.row();
        }
        return QSortFilterProxyModel::lessThan(left, right);
    }

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override
    {
        if (source_row == sourceModel()->rowCount(source_parent) - 1)
            return true;
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
};

}

UserList::UserList(UserListEndpoint& endpoint, QWidget *parent) :
    QWidget(parent),
    endpoint(endpoint),
    model(new UserListModel(endpoint.get_initial_users(), this)),
    model_proxy(new SortFilterProxyModel(this)),
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

void UserList::on_filter_users(const QString& pattern)
{
    model_proxy->setFilterRegularExpression(
            QRegularExpression(pattern, QRegularExpression::CaseInsensitiveOption));
}

void UserList::init()
{
    init_table_view();
    init_layout();
    connect(delegate, &UserListDelegate::clicked_add_user, this, &UserList::clicked_add_user);
    connect(user_list_filter, &UserListFilter::trigerred, this, &UserList::on_filter_users);
}

void UserList::init_table_view()
{
    constexpr int username_column = 0;

    model_proxy->setSourceModel(model);
    model_proxy->setFilterKeyColumn(username_column);

    table_view->setModel(model_proxy);
    table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table_view->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    table_view->setItemDelegate(delegate);
    table_view->setSortingEnabled(true);
    table_view->sortByColumn(username_column, Qt::AscendingOrder);
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
