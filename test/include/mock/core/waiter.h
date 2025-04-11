#pragma once

#include "gmock/gmock.h"

#include "core/waiter.h"

namespace matchmaker::core::mock {

class Waiter : public matchmaker::core::Waiter {
public:
    MOCK_METHOD(void, wait_for, (Duration duration), (const override));
};

}
