#include "main_mediator.h"

namespace matchmaker {

class UserDescriptorImpl : public gui::UserDescriptor {
public:
    inline static gui::UserDescriptor make(const core::User *user)
    {
        return gui::UserDescriptor::make(user, methods);
    }

private:
    inline static const core::User *get_user(const void *user) noexcept
    {
        return static_cast<const core::User *>(user);
    }

    inline static QString make_qstring_from_strview(std::string_view str)
    {
        return QString::fromUtf8(str.data(), static_cast<qsizetype>(str.size()));
    }

    inline static QString get_username(const void *user)
    {
        return QString::fromUtf8(get_user(user)->get_username());
    }

    inline static QString get_first_name(const void *user)
    {
        return make_qstring_from_strview(get_user(user)->get_name());
    }

    inline static QString get_last_name(const void *user)
    {
        return make_qstring_from_strview(get_user(user)->get_lastname());
    }

    static QStringList get_preferred_games(const void *user)
    {
        // auto games = get_user(user)->get_preferred_games();
        // QStringList game_names (games.size());
        // for (std::size_t i = 0; i < games.size(); ++i)
        //     game_names[i] = make_qstring_from_strview(games[i]->get_name());
        return { "Tic-Tac-Toe 3x3", "Tic-Tac-Toe 4x4",
                 "Tic-Tac-Toe 5x5", "Rock-Paper-Scissors" };
    }

    inline static const UserMethods methods = {
        &get_username,
        &get_first_name,
        &get_last_name,
        &get_preferred_games
    };
};

inline gui::UserDescriptor make_user_descriptor(const core::User *user)
{
    return UserDescriptorImpl::make(user);
}

MainMediator::MainMediator(
        core::UserRegistry& user_registry,
        core::MatchEngine& match_engine) :
    user_registry(user_registry),
    match_engine(match_engine)
{
}

gui::AddUserDialogError MainMediator::add_user(
        const QString& username,
        const QString& first_name,
        const QString& last_name,
        const QStringList& preferred_games,
        gui::AddUserCallback&& on_added_user)
{
    auto [user, error] = user_registry.register_user(
            username.toUtf8().toStdString(),
            first_name.toUtf8().toStdString(),
            last_name.toUtf8().toStdString(),
            // TODO: pass actual games, make game registry
            std::vector<core::Game *>(preferred_games.size(), nullptr));

    if (error == core::UserRegistry::ErrorNone)
        match_engine.add_user(*user,
            [this, user = user, on_added_user = std::move(on_added_user)]
            {
                on_added_user(make_user_descriptor(user));
            }
        );

    return error;
}

QStringList MainMediator::get_available_games() const
{
    return { "Tic-Tac-Toe 3x3", "Tic-Tac-Toe 4x4",
             "Tic-Tac-Toe 5x5", "Rock-Paper-Scissors" };
}

}
