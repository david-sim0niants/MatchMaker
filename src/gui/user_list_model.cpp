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

    if (index.row() == rowCount() - 1 && index.column() == 0)
        return "+Add User";
    else if (index.row() != rowCount() - 1)
        return "smth";
    else
        return QVariant();
}

int UserListModel::rowCount(const QModelIndex& parent) const
{
    return 1;
}

int UserListModel::columnCount(const QModelIndex& parent) const
{
    return 4;
}

}
