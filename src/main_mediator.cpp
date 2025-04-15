#include "main_mediator.h"

namespace matchmaker {

MainMediator::MainMediator(
        core::UserRegistry& user_registry,
        core::MatchEngine& match_engine) :
    user_registry(user_registry),
    match_engine(match_engine)
{
}

gui::AddUserDialogError MainMediator::add_user(
        const QString& username,
        const QString& name,
        const QString& last_name,
        const QStringList& preferred_games)
{
    auto [user, error] = user_registry.register_user(
            username.toUtf8().constData(),
            name.toUtf8().constData(),
            last_name.toUtf8().constData(), std::vector<core::Game *>(preferred_games.size(), nullptr));

    if (error == core::UserRegistry::ErrorNone)
        match_engine.add_user(*user, [this]{ /* TODO: call back to ui */ });

    return error;
}

QStringList MainMediator::get_available_games() const
{
    return { "Tic-Tac-Toe 3x3", "Tic-Tac-Toe 4x4",
             "Tic-Tac-Toe 5x5", "Rock-Paper-Scissors" };
}

}
