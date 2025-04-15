#pragma once

#include "core/match_engine.h"
#include "core/user_registry.h"
#include "gui/mainwindow.h"

namespace matchmaker {

class MainMediator : public gui::MainWindowEndpoint {
public:
    explicit MainMediator(
            core::UserRegistry& user_registry,
            core::MatchEngine& match_engine);

    gui::AddUserDialogError add_user(
            const QString& username,
            const QString& name,
            const QString& last_name,
            const QStringList& preferred_games) override;
    QStringList get_available_games() const override;

private:
    core::UserRegistry& user_registry;
    core::MatchEngine& match_engine;
};

}
