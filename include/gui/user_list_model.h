#pragma once

#include "gui/user_comparisons.h"
#include "gui/user_descriptor.h"

#include <QAbstractListModel>
#include <QMap>

namespace matchmaker::gui {

class UserListModel : public QAbstractListModel {
    Q_OBJECT;
public:
    UserListModel(QList<UserDescriptor>&& initial_users,
            Qt::SortOrder initial_order = Qt::AscendingOrder,
            QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    void add_user(UserDescriptor user);

private:
    int find_user(UserDescriptor) const;
    void reverse();
    template<UserProperty property> void sort();

    QList<UserDescriptor> users;
    UserCompare user_compare;
    Qt::SortOrder current_order;
};

}
