#pragma once

#include "gui/user_descriptor.h"

#include <QAbstractItemModel>

namespace matchmaker::gui {

class DashboardModel : public QAbstractItemModel {
    Q_OBJECT

private:
    struct Node {
        Node *parent;

        Node(Node *parent = nullptr) : parent(parent)
        {
        }
    };

    struct UserNode;

    struct GameNode : Node {
        QString name;
        std::vector<std::unique_ptr<UserNode>> user_nodes;

        explicit GameNode(QString&& name) : name(std::move(name))
        {
        }
    };

    struct UserNode : Node {
        UserDescriptor user;
        int rating = 0;

        explicit UserNode(GameNode *parent, UserDescriptor user) : Node(parent), user(user)
        {
        }
    };

public:
    using QAbstractItemModel::QAbstractItemModel;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QVariant headerData(int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int col, const QModelIndex& parent = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex&) const override;

    void update(QString&& game, UserDescriptor user, int rating);
    void add_user(UserDescriptor user);
    void rem_user(UserDescriptor user);

private:
    QModelIndex update_game(QString&& game);
    QModelIndex update_user(const QModelIndex& game_index, UserDescriptor user);

    void new_game_node(int index, QString&& game);
    void new_user_node(const QModelIndex& game_index, int user_index, UserDescriptor user);

    int find_game_node(const QString& game) const;
    static int find_user_node(const GameNode *game_node, const QString& username);

    std::vector<std::unique_ptr<GameNode>> game_nodes;
};

}
