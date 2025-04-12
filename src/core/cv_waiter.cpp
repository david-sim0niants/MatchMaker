#include "core/cv_waiter.h"

#include <chrono>

namespace matchmaker::core {

Duration CVWaiter::wait_for(Duration duration)
{
    auto start_time = std::chrono::steady_clock::now();
    std::unique_lock lock{mutex};
    cv.wait_for(lock, duration, [this] { return not waiting; });
    auto end_time = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<Duration>(end_time - start_time);
}

void CVWaiter::interrupt()
{
    std::scoped_lock lock{mutex};
    waiting = false;
    cv.notify_one();
}

}
