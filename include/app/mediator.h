#pragma once

#include "core/main_activity.h"
#include "gui/mainwindow.h"

namespace matchmaker::app {

class Mediator :
    public core::RatingMapObserver,
    public gui::AddUserDialogEndpoint,
    public gui::MainWindowEndpoint {

public:
    explicit Mediator(core::MainActivity& main_activity);

private:
    void notify_rating_change(
            const core::Game& game,
            const core::User& user,
            core::Rating rating) override;

    void start(gui::MainWindowControl& control) override;
    void stop() override;

    void save_game_ratings_to_file(const QString& game) override;
    void save_all_ratings_to_file() override;

    gui::AddUserDialogEndpoint& get_add_user_dialog_endpoint() override;

    gui::AddUserDialogError add_user(
            const QString& username,
            const QString& name,
            const QString& last_name,
            const QStringList& preferred_games) override;

    void rem_user(gui::UserDescriptor user) override;

    QStringList get_games() const override;

private:
    core::MainActivity& main_activity;
    gui::MainWindowControl *control;
};

}
