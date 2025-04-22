#include "app/mediator.h"
#include "core/user.h"
#include "gui/user_descriptor.h"

namespace matchmaker::app {

namespace {

class UserDescriptorImpl : public gui::UserDescriptor {
public:
    inline static gui::UserDescriptor make(const core::User *user) noexcept
    {
        return gui::UserDescriptor::make(user, ops);
    }

    inline static const core::User *obtain_user_from(UserDescriptor ud) noexcept
    {
        return cast_user(ops.get_user_from_descriptor(ud));
    }

private:
    inline static const core::User *cast_user(const void *user) noexcept
    {
        return static_cast<const core::User *>(user);
    }

    inline static QString make_qstring_from_strview(std::string_view str)
    {
        return QString::fromUtf8(str.data(), static_cast<qsizetype>(str.size()));
    }

    class OpsImpl : public Ops {
    public:
        std::string_view get_username(const void *user) const override
        {
            return cast_user(user)->get_username();
        }

        std::string_view get_first_name(const void *user) const override
        {
            return cast_user(user)->get_name();
        }

        std::string_view get_last_name(const void *user) const override
        {
            return cast_user(user)->get_lastname();
        }

        QStringList get_preferred_games(const void *user) const override
        {
            auto games = cast_user(user)->get_preferred_games();
            QStringList game_names (games.size());
            for (std::size_t i = 0; i < games.size(); ++i)
                game_names[i] = make_qstring_from_strview(games[i]->get_name());
            return game_names;
        }
    };

    inline static const OpsImpl ops;
};

inline gui::UserDescriptor make_user_descriptor(const core::User *user) noexcept
{
    return UserDescriptorImpl::make(user);
}

inline const core::User *obtain_user_from_descriptor(gui::UserDescriptor ud) noexcept
{
    return UserDescriptorImpl::obtain_user_from(ud);
}

}

Mediator::Mediator(core::MainActivity& main_activity) :
    main_activity(main_activity)
{
}

void Mediator::on_rating_change(
        const core::Game& game,
        const core::User& user,
        core::Rating rating)
{
    control->get_dashboard_control().update_rating(
            QString::fromStdString(game.get_name()),
            make_user_descriptor(&user),
            rating);
}

void Mediator::on_added_user(const core::User *user)
{
    control->add_user(make_user_descriptor(user));
}

void Mediator::on_removed_user(const core::User *user)
{
}

void Mediator::start(gui::MainWindowControl& control)
{
    this->control = &control;
    main_activity.set_rating_map_observer(this);
    main_activity.set_observer(this);
    main_activity.start();
}

void Mediator::stop()
{
    main_activity.stop();
    main_activity.set_observer(nullptr);
    main_activity.set_rating_map_observer(nullptr);
    this->control = nullptr;
}

void Mediator::save_game_ratings_to_file(const QString& game)
{
    std::string game_name = game.toStdString();
    main_activity.save_user_ratings_for_game(game_name);
}

void Mediator::save_all_ratings_to_file()
{
    main_activity.save_user_ratings();
}

gui::AddUserDialogEndpoint& Mediator::get_add_user_dialog_endpoint()
{
    return *this;
}

gui::AddUserDialogError Mediator::add_user(
        const QString& username,
        const QString& first_name,
        const QString& last_name,
        const QStringList& preferred_games)
{
    core::UserInfo user_info;

    std::string username_str = username.toStdString();
    user_info.username = username_str;
    user_info.name = first_name.toStdString();
    user_info.last_name = last_name.toStdString();

    user_info.preferred_games.reserve(preferred_games.size());
    for (auto&& game : preferred_games)
        user_info.preferred_games.emplace_back(game.toStdString());

    return main_activity.add_user(std::move(user_info));
}

void Mediator::rem_user(gui::UserDescriptor ud)
{
    const core::User *user = obtain_user_from_descriptor(ud);
    main_activity.rem_user(user);
}

QStringList Mediator::get_games() const
{
    auto games = main_activity.list_games();
    QStringList available_games (games.size());
    for (std::size_t i = 0; i < available_games.size(); ++i)
        available_games[i] = QString::fromStdString(games[i]->get_name());
    return available_games;
}

}
