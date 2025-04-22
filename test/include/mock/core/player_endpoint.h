#pragma once

#include "gmock/gmock.h"

#include "core/player.h"

namespace matchmaker::core::mock {

class PlayerEndpoint : public matchmaker::core::PlayerEndpoint {
public:
    MOCK_METHOD(void, request_match, (Player *player, const core::Game *game), (override));
    MOCK_METHOD(void, withdraw_match, (const Player *player), (override));
};

}
