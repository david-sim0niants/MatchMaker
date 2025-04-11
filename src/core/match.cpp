#include "core/match.h"

#include <cassert>

namespace matchmaker::core {

Match::Match(const Game& game) : game(game)
{
}

void Match::add_player(Player& player)
{
    if (! player_a)
        player_a = &player;
    else if (! player_b)
        player_b = &player;
    else
        throw MatchException("both players are present, no need for more");
}

void Match::start(MatchEndpoint& endpoint)
{
    if (game_instance)
        throw MatchException("the match has already started");
    this->endpoint = &endpoint;
    game_instance = game.launch(*player_a, *player_b, this);
}

void Match::stop()
{
    if (! game_instance)
        throw MatchException("the match has not started yet");
    game_instance->stop();
    game_instance = nullptr;
    endpoint = nullptr;
}

void Match::notify_finished(GameWinner winner)
{
    assert(endpoint != nullptr);
    switch (winner) {
    case GameWinner::None:
        endpoint->notify_match_finished(nullptr, nullptr);
        break;
    case GameWinner::PlayerA:
        endpoint->notify_match_finished(player_a, player_b);
        break;
    case GameWinner::PlayerB:
        endpoint->notify_match_finished(player_b, player_a);
        break;
    }
}

}
