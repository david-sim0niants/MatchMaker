#pragma once

#include "exception.h"
#include "core/game_instance.h"

#include <memory>

namespace matchmaker::core {

class Player;

class Game {
public:
    explicit Game(std::string&& name = {}) : name(std::move(name))
    {
    }

    virtual ~Game() = default;

    virtual std::unique_ptr<GameInstance> launch(
            const Player *player_a,
            const Player *player_b,
            GameInstanceObserver *observer = nullptr) const = 0;

    inline const std::string& get_name() const noexcept
    {
        return name;
    }

private:
    std::string name;
};

class GameException : public Exception {
    using Exception::Exception;
};

}
