#include "core/game_registry.h"
#include "misc/printing.h"

#include <filesystem>

namespace matchmaker::core {

GameRegistry::GameRegistry(
        std::string_view games_path_sv,
        ExecutableGameInfo *game_infos,
        std::size_t nr_game_infos)
{
    exe_games.reserve(nr_game_infos);
    std::filesystem::path games_path {games_path_sv};

    for (std::size_t i = 0; i < nr_game_infos; ++i) {
        const auto exe_game_path = games_path.parent_path() / game_infos[i].comm;

        if (! std::filesystem::exists(exe_game_path))
            throw GameRegistryException(
                    misc::stringify("failed to load '", exe_game_path, "' game"));

        exe_games.emplace_back(
                std::string(game_infos[i].name),
                std::string(game_infos[i].comm),
                exe_game_path.string());
    }

    games.reserve(exe_games.size());
    for (auto& exe_game : exe_games) {
        games.emplace_back(&exe_game);
        game_per_name.emplace(exe_game.get_name(), &exe_game);
    }
}

const core::Game *GameRegistry::get_game_by_name(std::string_view name) const noexcept
{
    auto it = game_per_name.find(name);
    if (it == game_per_name.end())
        return nullptr;
    else
        return it->second;
}

}
