#include "core/main_activity.h"

namespace matchmaker::core {

MainActivity::MainActivity(GameRegistry&& game_registry, std::string_view data_path)
    : game_registry(std::move(game_registry)), data_path(data_path)
{
}

void MainActivity::start()
{
    load_user_ratings();

    match_engine.keep_alive();
    thread = std::thread(&MainActivity::run, this);
    running = true;

    load_users();
}

void MainActivity::stop()
{
    match_engine.let_die();
    match_engine.rem_all_users();

    if (thread.joinable())
        thread.join();
    running = false;
}

void MainActivity::set_rating_map_observer(RatingMapObserver *rating_map_observer)
{
    if (running)
        match_engine.intercept(
            [rating_map_observer] (MatchEngineContext& context)
            {
                context.rating_map.set_observer(rating_map_observer);
            });
    else
        rating_map.set_observer(rating_map_observer);
}

void MainActivity::set_player_observer(PlayerObserver *player_observer)
{
    match_engine.set_player_observer(player_observer);
}

void MainActivity::save_user_ratings_for_game(std::string_view game)
{
    save_user_ratings_for_game_internal(game);
}

void MainActivity::save_user_ratings()
{
    for (auto *game : list_games())
        save_user_ratings_for_game_internal(game->get_name());
}

void MainActivity::run()
{
    rating_map = match_engine.run(std::exchange(rating_map, {}));
}

void MainActivity::load_users()
{
}

void MainActivity::save_users()
{
}

void MainActivity::load_user_ratings()
{
}

void MainActivity::save_user_ratings_for_game_internal(std::string_view game)
{
}

std::pair<const User *, UserRegistryError> MainActivity::register_user(UserInfo&& user_info)
{
    std::vector<const Game *> preferred_games;
    preferred_games.reserve(user_info.preferred_games.size());
    for (auto&& preferred_game : user_info.preferred_games)
        preferred_games.push_back(game_registry.get_game_by_name(preferred_game));

    return user_registry.register_user(
            user_info.username,
            std::move(user_info.name),
            std::move(user_info.last_name),
            std::move(preferred_games));
}

UserRegistryError MainActivity::unregister_user(const User& user)
{
    return user_registry.unregister_user(user);
}

}
