#pragma once

#include "core/rating_map.h"
#include "core/user_registry.h"
#include "core/game_registry.h"

#include <filesystem>
#include <functional>
#include <string_view>

namespace matchmaker::core {

class ResourceManager {
public:
    explicit ResourceManager(const GameRegistry *game_registry, std::string_view path);

    void load_user_registry(UserRegistry& user_registry,
            const std::function<void (User *)>& on_load_user);
    void save_user_registry(const UserRegistry& user_registry);

    void load_user_ratings_for_game(
            RatingMapPerGame& rating_map_per_game,
            const UserRegistry& user_registry);
    void save_user_ratings_for_game(const RatingMapPerGame& rating_map_per_game);

private:
    const GameRegistry *game_registry;
    std::filesystem::path path;

    static constexpr std::string_view
        user_list_fn =                 "user_list.txt",
        per_game_dashboard_fn_prefix = "dashboard_",
        per_game_dashboard_fn_suffix = ".txt";
};

}
