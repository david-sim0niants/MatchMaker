#include "app/mediator.h"

namespace matchmaker::app {

class UserDescriptorImpl : public gui::UserDescriptor {
public:
    inline static gui::UserDescriptor make(const core::User *user)
    {
        return gui::UserDescriptor::make(user, ops);
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

    class OpsImpl : public Ops {
    public:
        QString get_username(const void *user) const override
        {
            return QString::fromUtf8(get_user(user)->get_username());
        }

        QString get_first_name(const void *user) const override
        {
            return make_qstring_from_strview(get_user(user)->get_name());
        }

        QString get_last_name(const void *user) const override
        {
            return make_qstring_from_strview(get_user(user)->get_lastname());
        }

        QStringList get_preferred_games(const void *user) const override
        {
            auto games = get_user(user)->get_preferred_games();
            QStringList game_names (games.size());
            for (std::size_t i = 0; i < games.size(); ++i)
                game_names[i] = make_qstring_from_strview(games[i]->get_name());
            return game_names;
        }
    };

    inline static const OpsImpl ops;
};

inline gui::UserDescriptor make_user_descriptor(const core::User *user)
{
    return UserDescriptorImpl::make(user);
}

Mediator::Mediator(
        core::GameRegistry& game_registry,
        core::UserRegistry& user_registry,
        core::MatchEngine& match_engine) :
    game_registry(game_registry),
    user_registry(user_registry),
    match_engine(match_engine)
{
}

QList<gui::UserDescriptor> Mediator::get_initial_users() const
{
    return {};
}

gui::AddUserDialogError Mediator::add_user(
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
            find_games(preferred_games));

    if (error == core::UserRegistry::ErrorNone)
        match_engine.add_user(*user,
            [this, user = user, on_added_user = std::move(on_added_user)]
            {
                on_added_user(make_user_descriptor(user));
            }
        );

    return error;
}

QStringList Mediator::get_available_games() const
{
    auto games = game_registry.get_games();
    QStringList available_games (games.size());
    for (std::size_t i = 0; i < available_games.size(); ++i)
        available_games[i] = QString::fromStdString(games[i]->get_name());
    return available_games;
}

std::vector<const core::Game *>
    Mediator::find_games(const QStringList& preferred_game_names) const
{
    std::vector<const core::Game *> games;
    games.reserve(preferred_game_names.size());
    for (const QString& game_name : preferred_game_names)
        games.push_back(game_registry.get_game_by_name(game_name.toStdString()));
    return games;
}

}
