#pragma once

#include "core/exception.h"
#include "core/timeline_bound.h"
#include "core/timing.h"
#include "core/user.h"
#include "misc/printing.h"

namespace matchmaker::core {

class Player : public TimelineBound {
public:
    enum class State {
        Free = 0, Waiting, Busy
    };

    Player(User& user, Timeline& timeline);

    inline const User& get_user() const noexcept
    {
        return user;
    }

    void perform() override;
    void play(Game& game);
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
    void wait();

    void expect_state(State state);
    void change_state(State state);

    User& user;
    State state = State::Free;
    Time last_state_change_time;
    Game *current_game = nullptr;
};

class PlayerException : public Exception {
    using Exception::Exception;
};

std::string_view get_strview_of_player_state(Player::State state);

}

namespace matchmaker::misc {

template<> void print_to(std::ostream& os, const core::Player::State& state);

}
