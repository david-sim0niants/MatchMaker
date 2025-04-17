#include "gui/user_list_model.h"
#include "gui/user_property.h"

#include <algorithm>

namespace matchmaker::gui {

UserListModel::UserListModel(
        QList<UserDescriptor>&& initial_users,
        Qt::SortOrder initial_order,
        QObject *parent) :
    users(std::move(initial_users)),
    current_order(initial_order),
    QAbstractListModel(parent)
{
    std::sort(users.begin(), users.end(), user_compare);
}

Qt::ItemFlags UserListModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled;
}

QVariant UserListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (UserProperty(section)) {
        case UserProperty::Username:
            return "Username";
        case UserProperty::FirstName:
            return "First name";
        case UserProperty::LastName:
            return "Last name";
        case UserProperty::PreferredGames:
            return "Preferred games";
        default:
            return QVariant();
        }
    } else {
        if (section < rowCount() - 1)
            return section + 1;
        else
            return QVariant();
    }
}

QVariant UserListModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (index.row() == rowCount() - 1) {
        if (index.column() == 0)
            return "+Add User";
        else
            return QVariant();
    } else {
        int user_idx = index.row();
        UserDescriptor user = users[user_idx];

        switch (UserProperty(index.column())) {
        case UserProperty::Username:
            return user.get_username();
        case UserProperty::FirstName:
            return user.get_first_name();
        case UserProperty::LastName:
            return user.get_last_name();
        case UserProperty::PreferredGames:
            return user.get_preferred_games().join(',');
        default:
            return QVariant();
        }
    }
}

int UserListModel::rowCount(const QModelIndex& parent) const
{
    return users.size() + 1;
}

int UserListModel::columnCount(const QModelIndex& parent) const
{
    return 4;
}

void UserListModel::sort(int column, Qt::SortOrder order)
{
    if (column > 2)
        return;

    UserProperty property = UserProperty(column);
    if (user_compare.get_prioritized_property() == property) {
        if (current_order != order) {
            reverse();
            current_order = order;
        }
        return;
    }

    user_compare.prioritize(property);
    current_order = order;

    switch (property) {
    case UserProperty::Username:
        sort<UserProperty::Username>();
        break;
    case UserProperty::FirstName:
        sort<UserProperty::FirstName>();
        break;
    case UserProperty::LastName:
        sort<UserProperty::LastName>();
        break;
    default:
        break;
    }
}

void UserListModel::add_user(UserDescriptor user)
{
    int index = find_user(user);
    beginInsertRows(QModelIndex(), index, index);
    users.insert(users.begin() + index, user);
    endInsertRows();
}

int UserListModel::find_user(UserDescriptor user) const
{
    return current_order == Qt::AscendingOrder
        ?
        std::upper_bound(users.begin(), users.end(), user, user_compare) - users.begin()
        :
        std::upper_bound(users.rbegin(), users.rend(), user, user_compare) - users.rend();
}

void UserListModel::reverse()
{
    beginResetModel();
    std::reverse(users.begin(), users.end());
    endResetModel();
}

template<UserProperty property> void UserListModel::sort()
{
    UserPropertyCompare<property> partial_user_compare;
    beginResetModel();
    if (current_order == Qt::AscendingOrder)
        std::stable_sort(users.begin(), users.end(), partial_user_compare);
    else
        std::stable_sort(users.rbegin(), users.rend(), partial_user_compare);
    endResetModel();
}

}
