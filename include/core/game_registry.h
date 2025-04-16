#pragma once

#include "core/executable_game.h"
#include "core/executable_game_info.h"
#include "exception.h"

#include <cstddef>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace matchmaker::core {

class GameRegistry {
public:
    explicit GameRegistry(
            const char *exe_path,
            ExecutableGameInfo *exe_game_infos,
            std::size_t nr_exe_game_infos);

    inline const std::vector<const core::Game *>& get_games() const noexcept
    {
        return games;
    }

    const core::Game *get_game_by_name(std::string_view name) const noexcept;

private:
    std::vector<core::ExecutableGame> exe_games;
    std::vector<const core::Game *> games;
    std::unordered_map<std::string_view, const core::Game *> game_per_name;
};

class GameRegistryException : public Exception {
    using Exception::Exception;
};

}
