#include "gui/dashboard_model.h"

namespace matchmaker::gui {

Qt::ItemFlags DashboardModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
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

QVariant DashboardModel::data(const QModelIndex&, int role) const
{
    return QVariant();
}

QModelIndex DashboardModel::index(int, int, const QModelIndex&) const
{
    return QModelIndex();
}

int DashboardModel::rowCount(const QModelIndex& parent) const
{
    return 0;
}

int DashboardModel::columnCount(const QModelIndex& parent) const
{
    return 3;
}

QModelIndex DashboardModel::parent(const QModelIndex&) const
{
    return QModelIndex();
}

}
