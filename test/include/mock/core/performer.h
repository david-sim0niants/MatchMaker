#pragma once

#include "gmock/gmock.h"

#include "core/performer.h"

namespace matchmaker::core::mock {

class Performer : public matchmaker::core::Performer {
public:
    MOCK_METHOD(void, perform, (), (override));
};

}
