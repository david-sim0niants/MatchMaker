#pragma once

#include "core/match_engine.h"
#include "core/user_info.h"
#include "core/user_registry.h"
#include "core/game_registry.h"
#include "core/rating_map.h"
#include "core/cv_waiter.h"
#include "misc/prng.h"

namespace matchmaker::core {

class MainActivity {
public:
    MainActivity(GameRegistry&& game_registry, std::string_view data_path);

    void start();
    void stop();

    template<typename F = void(*)(const User& user), std::enable_if_t<std::is_invocable_v<F, const User&>, int> = 0>
    UserRegistryError add_user(UserInfo&& user_info, F on_add = [](const User& user){})
    {
        auto [user, error] = register_user(std::move(user_info));
        if (error == UserRegistry::ErrorNone)
            match_engine.add_user(*user, on_add);
        return error;
    }

    template<typename F = void(*)(const User& user), std::enable_if_t<std::is_invocable_v<F, const User&>, int> = 0>
    void rem_user(const User& user, F on_rem = [](const User& user){})
    {
        match_engine.rem_user(user, [this, on_rem](const User& user)
                {
                    on_rem(user);
                    unregister_user(user);
                });
    }

    void set_rating_map_observer(RatingMapObserver *rating_map_observer);
    void set_player_observer(PlayerObserver *player_observer);

    inline const auto& list_games() const noexcept
    {
        return game_registry.get_games();
    }

    void save_user_ratings_for_game(std::string_view game);
    void save_user_ratings();

private:
    void load_users();
    void save_users();

    void load_user_ratings();

    void save_user_ratings_for_game_internal(std::string_view game);

    std::pair<const User *, UserRegistryError> register_user(UserInfo&& user_info);
    UserRegistryError unregister_user(const User& user);

    std::string_view data_path;

    core::GameRegistry game_registry;
    core::UserRegistry user_registry;
    core::RatingMap rating_map;

    misc::PRNG prng;
    core::CVWaiter cv_waiter;

    core::MatchEngine match_engine {prng, cv_waiter};
    std::thread match_engine_thread;
    bool running = false;
};

}
