#pragma once

#include "core/exception.h"
#include "core/player.h"

namespace matchmaker::core {

class MatchException : public Exception {
public:
    using Exception::Exception;
};

class Match {
public:
    void add_player(Player& player)
    {
        if (! player_a)
            player_a = &player;
        else if (! player_b)
            player_b = &player;
        else
            throw MatchException("both players are present, no need for more");
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

    inline void reset() noexcept
    {
        player_a = player_b = nullptr;
    }

private:
    Player *player_a = nullptr, *player_b = nullptr;
};

}
