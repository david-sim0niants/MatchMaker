#include "core/user_registry.h"
#include <algorithm>
#include <cctype>

namespace matchmaker::core {

std::pair<const User *, UserRegistryError>
    UserRegistry::register_user(
        std::string_view username,
        std::string&& name,
        std::string&& last_name,
        std::vector<Game *>&& preferred_games)
{
    Error error = validate_user_info(username, name, last_name);
    if (error)
        return std::make_pair(nullptr, error);

    auto user_it = users.find(username);
    if (user_it != users.end())
        return std::make_pair(user_it->second.get(), ErrorUserAlreadyExists);

    user_it = users.emplace(username,
            std::make_unique<User>(
                username,
                std::move(name),
                std::move(last_name),
                std::move(preferred_games))).first;

    const User *user = user_it->second.get();
    return std::make_pair(user, ErrorNone);
}

UserRegistryError UserRegistry::unregister_user(std::string_view username)
{
    auto user_it = users.find(username);
    if (user_it == users.end())
        return ErrorUserDoesNotExist;

    users.erase(user_it);
    return ErrorNone;
}

UserRegistryError UserRegistry::validate_user_info(
        std::string_view username,
        std::string_view name,
        std::string_view last_name)
{
    Error error = ErrorNone;
    if (! UserRegistryPolicy::is_username_correct(username))
        error |= ErrorInvalidUsername;
    if (! UserRegistryPolicy::is_name_correct(name))
        error |= ErrorInvalidName;
    if (! UserRegistryPolicy::is_last_name_correct(last_name))
        error |= ErrorInvalidLastName;
    return error;
}

bool UserRegistryPolicy::is_username_correct(std::string_view username)
{
    if (username.empty() || username.size() > max_username_length)
        return false;
    return std::all_of(username.begin(), username.end(), ::isprint);
}

bool UserRegistryPolicy::is_name_correct(std::string_view name)
{
    if (name.empty())
        return false;
    if (! ('A' <= name.front() && name.front() <= 'Z'))
        return false;
    return std::all_of(name.begin(), name.end(), ::isalpha);
}

bool UserRegistryPolicy::is_last_name_correct(std::string_view last_name)
{
    if (last_name.empty())
        return false;
    if (! ('A' <= last_name.front() && last_name.front() <= 'Z'))
        return false;

    return std::all_of(last_name.begin(), last_name.end() - 1,
            [](char c){ return ::isalpha(c) || c == '\''; }) &&
        ::isalpha(last_name.back());
}

}
