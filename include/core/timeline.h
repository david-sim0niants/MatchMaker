#pragma once

#include "core/timing.h"
#include "core/performer.h"
#include "core/waiter.h"

#include <queue>

namespace matchmaker::core {

class Timeline {
public:
    explicit Timeline(Waiter& waiter, Time start_time = 0ms);

    void schedule_in(Duration duration, Performer& performer);
    void schedule_at(Time time, Performer& performer);

    void run();

    inline Time get_current_time() const noexcept
    {
        return current_time;
    }

private:
    void wait_until(Time time);

    Waiter& waiter;
    std::priority_queue<std::pair<Time, Performer *>> events;
    Time current_time;
};

}
