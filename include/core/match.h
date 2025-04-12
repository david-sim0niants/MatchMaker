#pragma once

#include "exception.h"
#include "core/player.h"
#include "core/game.h"

namespace matchmaker::core {

class MatchEndpoint;

class Match : GameInstanceObserver {
public:
    explicit Match(const Game& game);

    inline const Game& get_game() const noexcept
    {
        return game;
    }

    inline Player *get_player_a() const noexcept
    {
        return player_a;
    }

    inline Player *get_player_b() const noexcept
    {
        return player_b;
    }

    inline bool both_players_present() const noexcept
    {
        return player_a && player_b;
    }

    void add_player(Player& player);

    void start(MatchEndpoint& endpoint);
    void stop(Player *leaving_player = nullptr);

    void abort_by(Player& player);

private:
    void finish(GameWinner winner);
    void finish(Player *winner_player);
    void finalize();
    void notify_finished(GameWinner winner) override;

    const Game& game;
    Player *player_a = nullptr, *player_b = nullptr;
    MatchEndpoint *endpoint;
    std::unique_ptr<GameInstance> game_instance = nullptr;
};

class MatchEndpoint {
public:
    virtual void notify_match_finished(Match& match, Player *winner) = 0;
    virtual Timeline& get_timeline() const noexcept = 0;
};

class MatchException : public Exception {
public:
    using Exception::Exception;
};

}
