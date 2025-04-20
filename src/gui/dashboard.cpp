#include "gui/dashboard.h"
#include "gui/dashboard_model.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QMenu>

namespace matchmaker::gui {

Dashboard::Dashboard(QWidget *parent) :
    QWidget(parent),
    model(new DashboardModel(this)),
    tree_view(new QTreeView(this))
{
    init();
}

void Dashboard::update_rating(const QString& game, UserDescriptor user, int rating)
{
    emit updated_rating(game, user, rating);
}

void Dashboard::on_add_user(UserDescriptor user)
{
    model->add_user(user);
}

void Dashboard::on_rem_user(UserDescriptor user)
{
    model->rem_user(user);
}

void Dashboard::on_open_context_menu(const QPoint& pos)
{
    QModelIndex index = tree_view->indexAt(pos);

    if (! index.isValid())
        return;
    if (index.parent().isValid())
        return;

    QMenu menu {tree_view};
    QAction *save_to_file_action = new QAction("Save to file", this);
    menu.addAction(save_to_file_action);

    connect(save_to_file_action, &QAction::triggered,
            [this, index]()
            {
                emit clicked_save_game_ratings(model->get_game_at(index));
            });

    menu.exec(tree_view->viewport()->mapToGlobal(pos));
}

void Dashboard::init()
{
    init_table_view();
    init_layout();
    init_connections();
}

void Dashboard::init_table_view()
{
    tree_view->setModel(model);
    tree_view->header()->setSectionResizeMode(QHeaderView::Stretch);
    tree_view->setContextMenuPolicy(Qt::CustomContextMenu);
    tree_view->setSelectionMode(QAbstractItemView::SingleSelection);
    tree_view->setSelectionBehavior(QAbstractItemView::SelectItems);
    tree_view->show();
}

void Dashboard::init_layout()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(tree_view);

}

void Dashboard::init_connections()
{
    connect(this, &Dashboard::updated_rating, model, &DashboardModel::update);
    connect(tree_view, &QTreeView::customContextMenuRequested,
            this, &Dashboard::on_open_context_menu);
}

}
