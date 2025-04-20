#pragma once

#include "gui/user_descriptor.h"

#include <QAbstractListModel>

namespace matchmaker::gui {

class UserListModel : public QAbstractListModel {
    Q_OBJECT;
public:
    UserListModel(QObject *parent = nullptr);

    void reset(const QList<UserDescriptor>& users);
    UserDescriptor get_user_at(int index) const;
    void add_user(UserDescriptor user);
    void rem_user_at(int index);
    void rem_users(int from, int to);

private:
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

private:
    QList<UserDescriptor> users;
};

}
