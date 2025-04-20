#include "gui/user_descriptor.h"
#include "core/user.h"

namespace matchmaker::app {

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
