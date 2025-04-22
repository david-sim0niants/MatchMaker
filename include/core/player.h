#pragma once

#include "exception.h"
#include "core/timeline_object.h"
#include "core/timing.h"
#include "core/user.h"
#include "misc/prng.h"
#include "misc/printing.h"

namespace matchmaker::core {

class PlayerEndpoint; class PlayerObserver; class Match;

class Player : public TimelineObject {
public:
    enum class State {
        Created, Free, Waiting, Busy
    };

    Player(const User *user, PlayerEndpoint& endpoint, misc::PRNG& prng,
           PlayerObserver *observer = nullptr);

    inline const User *get_user() const noexcept
    {
        return user;
    }

    void init();
    void deinit();

    void play(Match& match);
    void finish_playing();

    inline State get_current_state() const noexcept
    {
        return state;
    }

    static constexpr Duration
        free_time = 2s,
        min_wait_time = 2s,
        max_wait_time = 3s;

private:
    void exec() override;

    void rest();
    void withdraw_match_and_rest();
    void select_game_and_request_match_and_wait();
    void select_game_and_request_match();
    void leave_match();

    void expect_state(State state);
    void change_state(State state);

    const User *user;
    PlayerEndpoint& endpoint;
    misc::PRNG& prng;

    State state = State::Free;
    Time last_state_change_time;
    Match *current_match = nullptr;

    PlayerObserver *observer = nullptr;
};

class PlayerEndpoint {
public:
    virtual void request_match(Player *player, const Game *game) = 0;
    virtual void withdraw_match(const Player *player) = 0;
};

class PlayerObserver {
public:
    virtual void on_state_change(const Player& player, Player::State state, Time time) = 0;
    virtual void on_match_start(const Player& player, const Match& match, Time time) = 0;
    virtual void on_match_leave(const Player& player, Time time) = 0;
};

class PlayerException : public Exception {
    using Exception::Exception;
};

std::string_view get_strview_of_player_state(Player::State state);

}

namespace matchmaker::misc {

template<> void print_to(std::ostream& os, const core::Player::State& state);

}
