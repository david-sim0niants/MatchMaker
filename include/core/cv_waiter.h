#pragma once

#include "core/waiter.h"

#include <mutex>
#include <condition_variable>

namespace matchmaker::core {

class CVWaiter : public Waiter {
public:
    Duration wait_for(Duration duration) override;
    void interrupt() override;

    static constexpr Duration max_wait_time = 24h;

private:
    std::mutex mutex;
    std::condition_variable cv;
    bool waiting = false;
};

}
