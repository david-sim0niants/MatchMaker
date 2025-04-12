#include "core/player.h"
#include "core/match.h"

#include <cassert>

namespace matchmaker::core {

Player::Player(User& user, PlayerEndpoint& endpoint, misc::PRNG prng) :
    user(user),
    endpoint(endpoint),
    prng(prng),
    state(Player::State::Created),
    last_state_change_time(0)
{
}

void Player::init()
{
    if (! Timeline::running())
        throw PlayerException("attempt to initialize a player outside timeline");
    rest();
}

void Player::play(Match& match)
{
    expect_state(State::Waiting);
    current_match = &match;
    change_state(State::Busy);
}

void Player::finish_playing()
{
    expect_state(State::Busy);
    current_match = nullptr;
    rest();
}

void Player::exec()
{
    if (state == State::Created)
        throw PlayerException("player must be initialized first");

    Time current_time = get_current_time();
    assert(current_time >= last_state_change_time);

    switch (state) {
    case State::Free:
        if (current_time - last_state_change_time >= free_time)
            select_game_and_request_match_and_wait();
        break;
    case State::Waiting:
        if (current_time - last_state_change_time >= wait_time)
            withdraw_match_and_rest();
    default:
        break;
    }
}

void Player::rest()
{
    change_state(State::Free);
    wait_for(free_time);
}

void Player::withdraw_match_and_rest()
{
    endpoint.withdraw_match(*this);
    rest();
}

void Player::select_game_and_request_match_and_wait()
{
    change_state(State::Waiting);
    select_game_and_request_match();

    if (get_current_state() == State::Waiting)
        wait_for(wait_time);
}

void Player::select_game_and_request_match()
{
    auto& preferred_games = user.get_preferred_games();
    if (preferred_games.empty())
        throw PlayerException( misc::stringify(
                    "the user '", user.get_username(), "' has no preferred games"));

    std::size_t game_idx = prng(std::size_t(), preferred_games.size() - 1);
    endpoint.request_match(*this, *preferred_games[game_idx]);
}

void Player::expect_state(State state)
{
    if (state != get_current_state())
        throw PlayerException( misc::stringify(
                    "invalid state - expected: ", state,
                    ", got: ", get_current_state() ) );
}

void Player::change_state(State state)
{
    this->state = state;
    last_state_change_time = get_current_time();
}

std::string_view get_strview_of_player_state(Player::State state)
{
    switch (state) {
    case Player::State::Free:
        return "Free";
    case Player::State::Waiting:
        return "Waiting";
    case Player::State::Busy:
        return "Busy";
    default:
        return "<invalid>";
    }
}

}

namespace matchmaker::misc {

template<> void print_to(std::ostream& os, const core::Player::State& state)
{
    os << core::get_strview_of_player_state(state);
}

}
