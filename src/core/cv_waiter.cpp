#include "core/cv_waiter.h"

#include <chrono>

namespace matchmaker::core {

Duration CVWaiter::wait_for(Duration duration)
{
    auto start_time = std::chrono::steady_clock::now();
    Duration partial_wait_duration = std::min(duration, max_wait_time);

    std::unique_lock lock{mutex};
    waiting = true;

    Duration time_passed = {};
    while (waiting) {
        if (cv.wait_for(lock, partial_wait_duration) == std::cv_status::no_timeout)
            continue;

        auto end_time = std::chrono::steady_clock::now();
        time_passed = std::chrono::duration_cast<Duration>(end_time - start_time);
        if (time_passed >= duration)
            break;
    }
    return time_passed;
}

void CVWaiter::interrupt()
{
    std::scoped_lock lock{mutex};
    waiting = false;
    cv.notify_one();
}

}
