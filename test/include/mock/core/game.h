#pragma once

#include "gmock/gmock.h"

#include "core/game.h"

namespace matchmaker::core::mock {

class Game : public matchmaker::core::Game {
public:
    MOCK_METHOD(Game::Winner, run, (const Player& player_a, const Player& player_b), (const override));
};

}

