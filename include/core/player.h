#pragma once

#include "exception.h"
#include "core/timeline_bound.h"
#include "core/timing.h"
#include "core/user.h"
#include "misc/prng.h"
#include "misc/printing.h"

namespace matchmaker::core {

class PlayerEndpoint;

class Player : public TimelineBound {
public:
    enum class State {
        Free = 0, Waiting, Busy
    };

    Player(User& user, Timeline& timeline, PlayerEndpoint& endpoint, misc::PRNG prng = {});

    inline const User& get_user() const noexcept
    {
        return user;
    }

    void perform() override;
    void play(const Game& game);
    void finish_playing();

    inline State get_current_state() const noexcept
    {
        return state;
    }

    static constexpr Duration
        free_time = 2s,
        wait_time = 3s;

private:
    void rest();
    void withdraw_match_and_rest();
    void select_game_and_request_match_and_wait();
    void select_game_and_request_match();

    void expect_state(State state);
    void change_state(State state);

    User& user;
    PlayerEndpoint& endpoint;
    misc::PRNG prng;

    State state = State::Free;
    Time last_state_change_time;
    const Game *current_game = nullptr;
};

class PlayerEndpoint {
public:
    virtual void request_match(Player& player, Game& game) = 0;
    virtual void withdraw_match(const Player& player) = 0;
};

class PlayerException : public Exception {
    using Exception::Exception;
};

std::string_view get_strview_of_player_state(Player::State state);

}

namespace matchmaker::misc {

template<> void print_to(std::ostream& os, const core::Player::State& state);

}
