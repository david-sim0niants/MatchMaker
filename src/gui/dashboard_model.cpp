#include "gui/dashboard_model.h"

namespace matchmaker::gui {

void DashboardModel::update(const QString& game, UserDescriptor user, int rating)
{
    QModelIndex game_index = update_game(QString(game));
    QModelIndex user_index = update_user(game_index, user);
    game_nodes[game_index.row()]->user_nodes[user_index.row()]->rating = rating;
    QModelIndex rating_index = user_index.siblingAtColumn(2);
    emit dataChanged(rating_index, rating_index);
}

void DashboardModel::add_user(UserDescriptor user)
{
    for (auto&& game : user.get_preferred_games())
        update(std::move(game), user, 0);
}

void DashboardModel::rem_user(UserDescriptor user)
{
    for (int i = 0; i < game_nodes.size(); ++i)
        rem_user_from_game(index(i, 0), user);
}

QModelIndex DashboardModel::update_game(QString&& game)
{
    int game_idx = find_game_node(game);

    assert(game_idx <= game_nodes.size());
    assert(game_idx >= 0);

    if (game_idx == game_nodes.size() || game_nodes[game_idx]->name != game)
        new_game_node(game_idx, std::move(game));

    return index(game_idx, 0);
}

QModelIndex DashboardModel::update_user(const QModelIndex& game_index, UserDescriptor user)
{
    GameNode *game_node = game_nodes[game_index.row()].get();

    int user_idx = find_user_node(game_node, user.get_username());
    if (user_idx == game_node->user_nodes.size() ||
        game_node->user_nodes[user_idx]->user.get_username() != user.get_username())
        new_user_node(game_index, user_idx, user);

    return index(user_idx, 1, game_index);
}

void DashboardModel::new_game_node(int index, QString&& game)
{
    beginInsertRows(QModelIndex(), index, index);
    game_nodes.insert(game_nodes.begin() + index, std::make_unique<GameNode>(std::move(game)));
    endInsertRows();
}

void DashboardModel::new_user_node(const QModelIndex& game_index,
        int user_index, UserDescriptor user)
{
    beginInsertRows(game_index, user_index, user_index);
    GameNode *game_node = game_nodes[game_index.row()].get();
    auto& user_nodes = game_node->user_nodes;
    user_nodes.insert(user_nodes.begin() + user_index,
                      std::make_unique<UserNode>(game_node, user));
    endInsertRows();
}

void DashboardModel::rem_user_from_game(const QModelIndex& game_index, UserDescriptor user)
{
    GameNode *game_node = game_nodes[game_index.row()].get();

    int user_idx = find_user_node(game_node, user.get_username());
    if (user_idx >= game_node->user_nodes.size() ||
        game_node->user_nodes[user_idx]->user.get_username() != user.get_username())
        return;

    beginRemoveRows(game_index, user_idx, user_idx);
    game_node->user_nodes.erase(game_node->user_nodes.begin() + user_idx);
    endRemoveRows();
}

int DashboardModel::find_game_node(const QString& game) const
{
    return std::lower_bound(game_nodes.begin(), game_nodes.end(), game,
            [](const auto& game_node, const QString& game_name)
            {
                return game_node->name < game_name;
            }) - game_nodes.begin();
}

int DashboardModel::find_user_node(const GameNode *game_node, std::string_view username)
{
    const auto& user_nodes = game_node->user_nodes;
    return std::lower_bound(user_nodes.begin(), user_nodes.end(), username,
            [](const auto& user_node, std::string_view username)
            {
                return user_node->user.get_username() < username;
            }) - user_nodes.begin();
}

Qt::ItemFlags DashboardModel::flags(const QModelIndex &index) const
{
    if (! index.isValid())
        return Qt::NoItemFlags;
    else if (! index.parent().isValid())
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    else if (index.column() > 0 && ! index.parent().parent().isValid())
        return Qt::ItemIsEnabled;
    else
        return Qt::NoItemFlags;
}

QVariant DashboardModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    switch (section) {
    case 0:
        return "Game";
    case 1:
        return "User";
    case 2:
        return "Rating";
    default:
        return QVariant();
    }
}

QVariant DashboardModel::data(const QModelIndex& index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (! index.parent().isValid()) {
        if (index.column() == 0)
            return static_cast<GameNode *>(index.internalPointer())->name;
        else
            return QVariant();
    }

    UserNode *user_node = static_cast<UserNode *>(index.internalPointer());
    if (index.column() == 1) {
        std::string_view username = user_node->user.get_username();
        return QString::fromUtf8(username.data(), username.size());
    } else if (index.column() == 2) {
        return user_node->rating;
    } else {
        return QModelIndex();
    }
}

QModelIndex DashboardModel::index(int row, int col, const QModelIndex& parent) const
{
    if ( ! parent.isValid()) {
        if (col == 0)
            return createIndex(row, 0, game_nodes[row].get());
    } else if (! parent.parent().isValid()) {
        GameNode *game_node = game_nodes[parent.row()].get();
        return createIndex(row, col, game_node->user_nodes[row].get());
    }
    return QModelIndex();
}

int DashboardModel::rowCount(const QModelIndex& parent) const
{
    if (! parent.isValid())
        return game_nodes.size();
    else if (! parent.parent().isValid())
        return game_nodes[parent.row()]->user_nodes.size();
    else
        return 0;
}

int DashboardModel::columnCount(const QModelIndex& parent) const
{
    return 3;
}

QModelIndex DashboardModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    Node *parent = static_cast<Node *>(index.internalPointer())->parent;
    if (parent == nullptr)
        return QModelIndex();

    GameNode *game_node = static_cast<GameNode *>(parent);
    return createIndex(find_game_node(game_node->name), 0, parent);
}

}
