#pragma once

#include "core/executable_game.h"

#include <optional>
#include <string_view>
#include <filesystem>

namespace matchmaker::core {

class ExecutableGameFactory {
public:
    ExecutableGameFactory(std::filesystem::path&& games_dir) :
        games_dir(games_dir)
    {
    }

    std::optional<ExecutableGame> create(std::string_view name)
    {
        if (std::filesystem::exists(games_dir / name))
            return ExecutableGame(std::string(name), (games_dir / name).string());
        else
            return std::nullopt;
    }

private:
    std::filesystem::path games_dir;
};

}
