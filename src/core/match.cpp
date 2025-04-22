#include "core/match.h"

#include <cassert>

namespace matchmaker::core {

Match::Match(const Game *game) : game(game)
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
    game_instance = game->launch(player_a, player_b, this);

    player_a->play(*this);
    player_b->play(*this);
}

void Match::stop(Player *leaving_player)
{
    if (! game_instance || ! endpoint)
        throw MatchException("cannot stop, the match has not started yet");

    game_instance->stop();

    if (leaving_player != nullptr) {
        if (leaving_player == get_player_a())
            finish(player_b);
        else
            finish(player_a);
    } else {
        finish(nullptr);
    }
}

void Match::finish(GameWinner winner)
{
    switch (winner) {
    case GameWinner::None:
        finish(nullptr);
        break;
    case GameWinner::PlayerA:
        finish(get_player_a());
        break;
    case GameWinner::PlayerB:
        finish(get_player_b());
        break;
    }
}

void Match::finish(Player *winner_player)
{
    MatchEndpoint *endpoint = this->endpoint;
    finalize();
    endpoint->notify_match_finished(*this, winner_player);
}

void Match::finalize()
{
    game_instance = nullptr;
    endpoint = nullptr;

    player_a->finish_playing();
    player_b->finish_playing();
}

void Match::notify_finished(GameWinner winner)
{
    assert(endpoint != nullptr);
    endpoint->get_timeline().post([this, winner]() { finish(winner); });
}

}
