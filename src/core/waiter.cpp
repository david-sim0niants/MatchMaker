#include "core/waiter.h"

namespace matchmaker::core {

void SleepingWaiter::wait_for(Duration duration) const
{
    std::this_thread::sleep_for(duration);
}

}
