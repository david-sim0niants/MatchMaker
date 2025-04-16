#include "gui/user_list_model.h"

namespace matchmaker::gui {

Qt::ItemFlags UserListModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsEnabled;
}

QVariant UserListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "Username";
        case 1:
            return "First name";
        case 2:
            return "Last name";
        case 3:
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

        switch (index.column()) {
        case 0:
            return user.get_username();
        case 1:
            return user.get_first_name();
        case 2:
            return user.get_last_name();
        case 3:
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

void UserListModel::add_user(UserDescriptor user)
{
    int row = rowCount() - 1;
    beginInsertRows(QModelIndex(), row, row);
    users.append(user);
    endInsertRows();
}

}
