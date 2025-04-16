#pragma once

#include "core/game_registry.h"
#include "core/user_registry.h"
#include "core/match_engine.h"
#include "gui/mainwindow.h"

namespace matchmaker::app {

class Mediator : public gui::MainWindowEndpoint {
public:
    explicit Mediator(
            core::GameRegistry& game_registry,
            core::UserRegistry& user_registry,
            core::MatchEngine& match_engine);

    QList<gui::UserDescriptor> get_initial_users() const override;

    gui::AddUserDialogError add_user(
            const QString& username,
            const QString& name,
            const QString& last_name,
            const QStringList& preferred_games,
            gui::AddUserCallback&& on_added_user) override;
    QStringList get_available_games() const override;

private:
    std::vector<const core::Game *>
        find_games(const QStringList& preferred_game_names) const;

    core::GameRegistry& game_registry;
    core::UserRegistry& user_registry;
    core::MatchEngine& match_engine;
};


}
