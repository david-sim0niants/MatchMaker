#pragma once

#include "exception.h"
#include "core/game_instance.h"

#include <memory>

namespace matchmaker::core {

class Player;

class Game {
public:
    virtual ~Game() = default;

    virtual std::unique_ptr<GameInstance> launch(
            const Player& player_a,
            const Player& player_b,
            GameInstanceObserver *observer = nullptr) const = 0;
};

class GameException : public Exception {
    using Exception::Exception;
};

}
