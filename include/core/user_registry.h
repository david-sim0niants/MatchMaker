#pragma once

#include "user.h"

#include <unordered_map>
#include <vector>

namespace matchmaker::core {

class UserRegistry {
public:
    enum : int {
        ErrorNone = 0,
        ErrorInvalidUsername = 1,
        ErrorInvalidName = 2,
        ErrorInvalidLastName = 4,
        ErrorNoPreferredGames = 8,
        ErrorUserAlreadyExists = 16,
        ErrorUserDoesNotExist = 17,
    };
    using Error = int;

    std::pair<const User *, Error>
        register_user(
            std::string_view username,
            std::string&& name,
            std::string&& last_name,
            std::vector<const Game *>&& preferred_games);
    Error unregister_user(std::string_view username);

    inline Error unregister_user(const core::User& user) noexcept
    {
        return unregister_user(user.get_username());
    }

    const User *get_user_by_username(std::string_view username);

    std::vector<const User *> collect() const;

    static Error validate_user_info(
            std::string_view username,
            std::string_view name,
            std::string_view last_name);

private:
    std::unordered_map<std::string_view, std::unique_ptr<User>> users;
};

using UserRegistryError = UserRegistry::Error;

class UserRegistryPolicy {
public:
    static constexpr auto max_username_length = User::max_username_length;

    static bool is_username_correct(std::string_view username);
    static bool is_name_correct(std::string_view name);
    static bool is_last_name_correct(std::string_view last_name);
};

}
