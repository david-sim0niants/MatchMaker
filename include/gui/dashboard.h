#pragma once

#include "gui/dashboard_model.h"

#include <QWidget>
#include <QTreeView>

namespace matchmaker::gui {

class Dashboard : public QWidget {
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);

private:
    gui::DashboardModel *model;
    QTreeView *tree_view;
};

}
