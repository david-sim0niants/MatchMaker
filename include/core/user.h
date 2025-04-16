#pragma once

#include "game.h"

#include <string>
#include <string_view>
#include <vector>

namespace matchmaker::core {

class User {
public:
    User(std::string_view username, std::string&& name, std::string&& last_name,
         std::vector<const Game *>&& preferred_games);

    inline const char *get_username() const noexcept
    {
        return username;
    }

    inline std::size_t get_username_length() const noexcept
    {
        return username_length;
    }

    inline std::string_view get_name() const noexcept
    {
        return name;
    }

    inline std::string_view get_lastname() const noexcept
    {
        return last_name;
    }

    inline const std::vector<const Game *>& get_preferred_games() const noexcept
    {
        return preferred_games;
    }

    static constexpr std::size_t max_username_length = 16;

private:
    char username[max_username_length + 1] {};
    std::size_t username_length;
    std::string name, last_name;
    std::vector<const Game *> preferred_games;
};

}
