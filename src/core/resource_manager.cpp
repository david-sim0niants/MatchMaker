#include "core/resource_manager.h"

#include <algorithm>
#include <fstream>
#include <string>

namespace matchmaker::core {

ResourceManager::ResourceManager(const GameRegistry *game_registry, std::string_view path) :
    path(path), game_registry(game_registry)
{
}

void ResourceManager::load_user_registry(UserRegistry& user_registry,
        const std::function<void (User *)>& on_load_user)
{
    std::ifstream user_list_file {path / user_list_fn};
    if (! user_list_file)
        return;

    char username[User::max_username_length];
    user_list_file >> username;

    std::size_t username_length =
        std::find_if_not(std::begin(username), std::end(username), ::isprint)
        - std::begin(username);

    std::string name;
    user_list_file >> name;

    std::string last_name;
    user_list_file >> last_name;
}

void ResourceManager::save_user_registry(const UserRegistry& user_registry)
{
}

void ResourceManager::load_user_ratings_for_game(
        RatingMapPerGame& rating_map_per_game,
        const UserRegistry& user_registry)
{
}

void ResourceManager::save_user_ratings_for_game(const RatingMapPerGame& rating_map_per_game)
{
}

}
