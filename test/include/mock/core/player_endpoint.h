#pragma once

#include "gmock/gmock.h"

#include "core/player.h"

namespace matchmaker::core::mock {

class PlayerEndpoint : public matchmaker::core::PlayerEndpoint {
public:
    MOCK_METHOD(void, request_match, (Player& player, core::Game& game), (override));
};

}
