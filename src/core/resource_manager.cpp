#include "core/resource_manager.h"
#include "core/serialization.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

namespace matchmaker::core {

static void save_user_data(std::ostream& os, const User *user);

ResourceManager::ResourceManager(const GameRegistry *game_registry, std::string_view path) :
    path(path), game_registry(game_registry)
{
}

void ResourceManager::load_user_registry(UserRegistry& user_registry,
        const std::function<void (const User *, UserRegistryError)>& on_load_user)
{
    std::ifstream user_list_file {path / user_list_fn};

    std::string name, last_name, preferred_game_name;

    while (user_list_file) {

        UserDeserializer deserializer (user_list_file);

        char username[User::max_username_length] {};
        std::size_t username_length;

        deserializer.read_username(username, username_length);
        if (username_length == 0)
            continue;

        deserializer.read_name(name);
        deserializer.read_last_name(last_name);

        std::vector<const Game *> preferred_games;
        while (deserializer.read_preferred_game(preferred_game_name)) {
            const Game *preferred_game = game_registry->get_game_by_name(preferred_game_name);
            if (preferred_game)
                preferred_games.push_back(preferred_game);
        }

        auto [user, error] = user_registry.register_user(
                std::string_view(username, username_length),
                std::move(name), std::move(last_name), std::move(preferred_games));

        if (on_load_user)
            on_load_user(user, error);
    }
}

void ResourceManager::save_user_registry(const UserRegistry& user_registry)
{
    std::filesystem::path user_list_path = path / user_list_fn;
    std::ofstream user_list_file {path / user_list_fn};

    auto users = user_registry.collect();
    std::sort(users.begin(), users.end(),
            [](const User *user_a, const User *user_b)
            {
                return user_a->get_username() < user_b->get_username();
            });

    for (const User *user : users)
        save_user_data(user_list_file, user);

    user_list_file.seekp(0, std::ios_base::end);
    std::filesystem::resize_file(user_list_path, user_list_file.tellp());
}

void ResourceManager::load_user_ratings_for_game(
        RatingMapPerGame& rating_map_per_game,
        const UserRegistry& user_registry)
{
}

void ResourceManager::save_user_ratings_for_game(const RatingMapPerGame& rating_map_per_game)
{
}

static void save_user_data(std::ostream& os, const User *user)
{
    UserSerializer serializer(os);
    serializer.write_username(user->get_username());
    serializer.write_name(user->get_name());
    serializer.write_last_name(user->get_lastname());

    for (const Game *game : user->get_preferred_games())
        serializer.write_preferred_game(game->get_name());

    serializer.finish();
}

}
