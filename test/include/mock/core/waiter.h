#pragma once

#include "gmock/gmock.h"

#include "core/waiter.h"

namespace matchmaker::core::mock {

class Waiter : public matchmaker::core::Waiter {
public:
    MOCK_METHOD(Duration, wait_for, (Duration duration), (override));
    MOCK_METHOD(void, interrupt, (), (override));
};

}
