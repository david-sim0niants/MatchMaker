#pragma once

#include "core/match_engine.h"
#include "core/user_info.h"
#include "core/user_registry.h"
#include "core/game_registry.h"
#include "core/resource_manager.h"
#include "core/rating_map.h"
#include "core/cv_waiter.h"
#include "misc/prng.h"

#include <utility>

namespace matchmaker::core {

class MainActivityObserver;

class MainActivity {
public:
    MainActivity(misc::PRNG& prng,
            ExecutableGameInfo *game_infos, std::size_t nr_game_infos,
            std::string_view game_path, std::string_view data_path);

    void start();
    void stop();

    UserRegistryError add_user(UserInfo&& user_info);
    void rem_user(const User& user);

    void set_rating_map_observer(RatingMapObserver *rating_map_observer);
    void set_player_observer(PlayerObserver *player_observer);
    void set_observer(MainActivityObserver *observer);

    inline const auto& list_games() const noexcept
    {
        return game_registry.get_games();
    }

    void save_user_ratings();
    void save_user_ratings_for_game(std::string_view game);

private:
    void run();

    void load_user_registry();
    void save_user_registry();

    void add_user_to_match_engine(const User& user);

    void load_user_ratings();
    void save_user_ratings_for_game_internal(std::string_view game);

    std::pair<const User *, UserRegistryError> register_user(UserInfo&& user_info);
    UserRegistryError unregister_user(const User& user);

    CVWaiter cv_waiter;

    MainActivityObserver *observer = nullptr;

    GameRegistry game_registry;
    UserRegistry user_registry;
    RatingMap rating_map;

    ResourceManager resources;
    MatchEngine match_engine;

    std::thread thread;
    bool running = false;
};

class MainActivityObserver {
public:
    virtual void on_added_user(const User& user) = 0;
    virtual void on_removed_user(const User& user) = 0;
};

}
