#include "core/main_activity.h"

namespace matchmaker::core {

MainActivity::MainActivity(
        misc::PRNG& prng,
        ExecutableGameInfo *game_infos, std::size_t nr_game_infos,
        std::string_view game_path, std::string_view data_path) :
    game_registry(game_path, game_infos, nr_game_infos),
    resources(&game_registry, data_path),
    match_engine(prng, cv_waiter)
{
}

void MainActivity::start()
{
    load_user_registry();

    match_engine.keep_alive();
    thread = std::thread([this]
        {
            rating_map = match_engine.run(std::exchange(rating_map, {}));
        });
    running = true;

    load_user_ratings();
}

void MainActivity::stop()
{
    match_engine.let_die();
    match_engine.rem_all_users();

    if (thread.joinable())
        thread.join();
    running = false;

    save_user_registry();

    user_registry = UserRegistry();
    rating_map = RatingMap();
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

void MainActivity::set_observer(MainActivityObserver *observer)
{
    if (running)
        match_engine.intercept([this, observer](auto&&...) { this->observer = observer; });
    else
        this->observer = observer;
}

UserRegistryError MainActivity::add_user(UserInfo&& user_info)
{
    auto [user, error] = register_user(std::move(user_info));
    if (error == UserRegistry::ErrorNone)
        add_user_to_match_engine(user);
    return error;
}

void MainActivity::rem_user(const User *user)
{
    match_engine.rem_user(user,
        [this] (MatchEngineContext& context, const User *user)
        {
            context.rating_map.rem_user(user);
            if (observer)
                observer->on_removed_user(user);
            unregister_user(user);
        });
}

void MainActivity::save_user_ratings()
{
    if (running)
        match_engine.intercept_async(
                [this](MatchEngineContext& context)
                {
                    save_user_ratings_internal(context.rating_map);
                });
    else
        save_user_ratings_internal(rating_map);
}

void MainActivity::save_user_ratings_for_game(std::string_view game_name)
{
    const Game *game = game_registry.get_game_by_name(game_name);
    if (game)
        save_user_ratings_for_game(game);
}

void MainActivity::save_user_ratings_for_game(const Game *game)
{
    if (running)
        match_engine.intercept_async(
            [this, game](MatchEngineContext& context)
            {
                save_user_ratings_for_game_internal(game, context.rating_map);
            });
    else
        save_user_ratings_for_game_internal(game, this->rating_map);
}

void MainActivity::add_user_to_match_engine(const User *user)
{
    match_engine.add_user(user,
        [this] (auto&, const User *user)
        {
            if (observer)
                observer->on_added_user(user);
        });
}

void MainActivity::load_user_registry()
{
    resources.load_user_registry(user_registry,
        [this](const User *user, UserRegistryError error)
        {
            if (user)
                add_user_to_match_engine(user);
        });
}

void MainActivity::save_user_registry()
{
    resources.save_user_registry(user_registry);
}

void MainActivity::load_user_ratings()
{
    if (running)
        match_engine.intercept_async(
            [this](MatchEngineContext& context)
            {
                resources.load_user_ratings(context.rating_map, user_registry);
            });
    else
        resources.load_user_ratings(rating_map, user_registry);
}

void MainActivity::save_user_ratings_internal(const RatingMap& rating_map)
{
    resources.save_user_ratings(rating_map);
}

void MainActivity::save_user_ratings_for_game_internal(
        const Game *game, const RatingMap& rating_map)
{
    const RatingMapPerGame *rating_map_per_game = rating_map.get_rating_map_for_game(game);
    if (rating_map_per_game)
        resources.save_user_ratings_for_game(*rating_map_per_game);
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

UserRegistryError MainActivity::unregister_user(const User *user)
{
    return user_registry.unregister_user(user);
}

}
