#pragma once

#include "core/match_engine.h"
#include "core/user_info.h"
#include "core/user_registry.h"
#include "core/game_registry.h"
#include "core/rating_map.h"
#include "core/cv_waiter.h"
#include "misc/prng.h"

#include <utility>

namespace matchmaker::core {

class MainActivity {
public:
    MainActivity(GameRegistry&& game_registry, std::string_view data_path);

    void start();
    void stop();

    template<typename F = void(*)(const User& user)>
    UserRegistryError add_user(UserInfo&& user_info, F on_add = [](const User& user){})
    {
        auto [user, error] = register_user(std::move(user_info));
        if (error == UserRegistry::ErrorNone)
            match_engine.add_user(*user, [this, on_add]
                    (auto&, const User& user)
                    {
                        on_add(user);
                    });
        return error;
    }

    template<typename F = void(*)(const User& user)>
    void rem_user(const User& user, F on_rem = [](const User& user){})
    {
        match_engine.rem_user(user, [this, on_rem]
                (MatchEngineContext& context, const User& user)
                {
                    on_rem(user);
                    context.rating_map.rem_user(user);
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
    void run();

    void load_users();
    void save_users();

    void load_user_ratings();

    void save_user_ratings_for_game_internal(std::string_view game);

    std::pair<const User *, UserRegistryError> register_user(UserInfo&& user_info);
    UserRegistryError unregister_user(const User& user);

    misc::PRNG prng;
    CVWaiter cv_waiter;

    std::string_view data_path;

    GameRegistry game_registry;
    UserRegistry user_registry;
    RatingMap rating_map;

    MatchEngine match_engine {prng, cv_waiter};

    std::thread thread;
    bool running = false;
};

}
