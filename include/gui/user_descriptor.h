#pragma once

#include <QString>
#include <QStringList>

namespace matchmaker::gui {

class UserDescriptor {
public:
    inline QString get_username() const
    {
        return ops->get_username(user);
    }

    inline QString get_first_name() const
    {
        return ops->get_first_name(user);
    }

    inline QString get_last_name() const
    {
        return ops->get_last_name(user);
    }

    inline QStringList get_preferred_games() const
    {
        return ops->get_preferred_games(user);
    }

protected:
    class Ops {
    public:
        virtual QString get_username(const void *user) const = 0;
        virtual QString get_first_name(const void *user) const = 0;
        virtual QString get_last_name(const void *user) const = 0;
        virtual QStringList get_preferred_games(const void *user) const = 0;

        inline const void *get_user_from_descriptor(UserDescriptor descriptor) const noexcept
        {
            return descriptor.user;
        }
    };

    inline static UserDescriptor make(const void *user, const Ops& ops) noexcept
    {
        return UserDescriptor(user, ops);
    }

    inline const void *get_user() const noexcept
    {
        return user;
    }

private:
    UserDescriptor(const void *user, const Ops& ops) noexcept :
        user(user), ops(&ops)
    {
    }

    const void *user;
    const Ops *ops;
};

}
