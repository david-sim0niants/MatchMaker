#pragma once

#include <QString>
#include <QStringList>

namespace matchmaker::gui {

class UserDescriptor {
public:
    inline QString get_username() const
    {
        return methods.get_username(user);
    }

    inline QString get_first_name() const
    {
        return methods.get_first_name(user);
    }

    inline QString get_last_name() const
    {
        return methods.get_last_name(user);
    }

    inline QStringList get_preferred_games() const
    {
        return methods.get_preferred_games(user);
    }

protected:
    class UserMethods {
    public:
        QString (*get_username)(const void *user);
        QString (*get_first_name)(const void *user);
        QString (*get_last_name)(const void *user);
        QStringList (*get_preferred_games)(const void *user);
    };

    inline static UserDescriptor make(const void *user, const UserMethods& methods)
    {
        return UserDescriptor(user, methods);
    }

private:
    UserDescriptor(const void *user, const UserMethods& impl) :
        user(user), methods(impl)
    {
    }

    const void *user;
    const UserMethods& methods;
};


}
