#pragma once

#include <QAbstractItemModel>

namespace matchmaker::gui {

class DashboardModel : public QAbstractItemModel {
    Q_OBJECT
public:
    using QAbstractItemModel::QAbstractItemModel;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    QVariant headerData(int section,
            Qt::Orientation orientation,
            int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex&, int role = Qt::DisplayRole) const override;

    QModelIndex index(int, int, const QModelIndex& = QModelIndex()) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex&) const override;
};

}
