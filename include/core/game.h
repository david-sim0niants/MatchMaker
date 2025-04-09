#pragma once

namespace matchmaker::core {

class Player;

class Game {
public:
    enum class Winner {
        None, PlayerA, PlayerB
    };

    virtual Winner run(const Player& player_a, const Player& player_b) const = 0;
};

}
