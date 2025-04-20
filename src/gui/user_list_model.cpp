#include "gui/user_list_model.h"

namespace matchmaker::gui {

static QString make_qstring_from_strview(std::string_view strview)
{
    return QString::fromUtf8(strview.data(), strview.size());
}

UserListModel::UserListModel(QObject *parent) : QAbstractListModel(parent)
{
}

void UserListModel::reset(const QList<UserDescriptor>& users)
{
    beginResetModel();
    this->users = users;
    endResetModel();
}

UserDescriptor UserListModel::get_user_at(int index) const
{
    return users[index];
}

void UserListModel::add_user(UserDescriptor user)
{
    beginInsertRows(QModelIndex(), users.size(), users.size());
    users.append(user);
    endInsertRows();
}

void UserListModel::rem_user_at(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    users.removeAt(index);
    endRemoveRows();
}

void UserListModel::rem_users(int from, int to)
{
    beginRemoveRows(QModelIndex(), from, to);
    users.erase(users.begin() + from, users.begin() + to + 1);
    endRemoveRows();
}

Qt::ItemFlags UserListModel::flags(const QModelIndex &index) const
{
    if (index.row() < rowCount() - 1)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    return {};
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

    if (index.row() >= rowCount() - 1)
        return QVariant();

    int user_idx = index.row();
    UserDescriptor user = users[user_idx];

    switch (index.column()) {
    case 0:
        return make_qstring_from_strview(user.get_username());
    case 1:
        return make_qstring_from_strview(user.get_first_name());
    case 2:
        return make_qstring_from_strview(user.get_last_name());
    case 3:
        return user.get_preferred_games().join(',');
    default:
        return QVariant();
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

}
