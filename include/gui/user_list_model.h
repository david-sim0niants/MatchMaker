#pragma once

#include <QAbstractListModel>

namespace matchmaker::gui {

class UserListModel : public QAbstractListModel {
    Q_OBJECT;
public:
    using QAbstractListModel::QAbstractListModel;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant headerData(int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
};

}
