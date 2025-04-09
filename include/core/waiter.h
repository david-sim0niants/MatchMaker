#pragma once

#include "core/timing.h"
#include <thread>

namespace matchmaker::core {

class Waiter {
public:
    virtual void wait_for(Duration duration) const = 0;
};

class SleepingWaiter : public Waiter {
    void wait_for(Duration duration) const override
    {
        std::this_thread::sleep_for(duration);
    }
};

}
