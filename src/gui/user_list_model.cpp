#include "gui/user_list_model.h"

namespace matchmaker::gui {

enum class UserProperty {
    Username = 0,
    FirstName = 1,
    LastName = 2,
    PreferredGames = 3,
};

UserListModel::UserListModel(QList<UserDescriptor>&& initial_users, QObject *parent) :
    QAbstractListModel(parent),
    users(std::move(initial_users))
{
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

    if (index.row() >= rowCount() - 1)
        return QVariant();

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
    beginInsertRows(QModelIndex(), users.size(), users.size());
    users.append(user);
    endInsertRows();
}

}
