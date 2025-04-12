#pragma once

#include "core/game.h"

#include <string>

namespace matchmaker::core {

class ExecutableGame : public Game {
public:
    ExecutableGame(std::string&& name, std::string&& path);

    inline std::string_view get_name() const noexcept override
    {
        return name;
    }

    std::unique_ptr<GameInstance> launch(
            const Player& player_a,
            const Player& player_b,
            GameInstanceObserver *observer = nullptr) const override;
private:
    std::string name;
    std::string path;
};

class ExecutableGameException : public GameException {
public:
    using GameException::GameException;
};

}
