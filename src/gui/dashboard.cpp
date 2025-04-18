#include "gui/dashboard.h"
#include "gui/dashboard_model.h"

#include <QVBoxLayout>
#include <QHeaderView>

namespace matchmaker::gui {

Dashboard::Dashboard(QWidget *parent) :
    QWidget(parent), model(new DashboardModel(this)), tree_view(new QTreeView(this))
{
    tree_view->setModel(model);
    tree_view->header()->setSectionResizeMode(QHeaderView::Stretch);
    tree_view->show();

    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(tree_view);
}

}
