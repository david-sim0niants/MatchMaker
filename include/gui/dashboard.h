#pragma once

#include <QWidget>
#include <QTreeView>

namespace matchmaker::gui {

class Dashboard : public QWidget {
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);

private:
    QTreeView *tree_view;
};

}
