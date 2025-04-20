#pragma once

#include "gui/dashboard_model.h"
#include "gui/user_descriptor.h"

#include <QWidget>
#include <QTreeView>

namespace matchmaker::gui {

class DashboardControl {
public:
    virtual void update_rating(const QString& game, UserDescriptor user, int rating) = 0;
};

class Dashboard : public QWidget, public DashboardControl {
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);

    void update_rating(const QString& game, UserDescriptor user, int rating) override;

signals:
    void clicked_save_game_ratings(const QString& game);

public slots:
    void on_add_user(UserDescriptor user);
    void on_rem_user(UserDescriptor user);

private:
signals:
    void updated_rating(const QString& game, UserDescriptor user, int rating);

private slots:
    void on_open_context_menu(const QPoint& pos);

private:
    DashboardModel *model;
    QTreeView *tree_view;
};

}
