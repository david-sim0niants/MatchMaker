#pragma once

#include "core/timing.h"

namespace matchmaker::core {

class Waiter {
public:
    virtual Duration wait_for(Duration duration) = 0;
    virtual void interrupt() = 0;
};

}
