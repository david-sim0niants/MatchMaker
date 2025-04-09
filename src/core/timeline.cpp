#include "core/timeline.h"

#include <cassert>

namespace matchmaker::core {

Timeline::Timeline(Waiter& waiter, Time start_time) :
    waiter(waiter), current_time(start_time)
{
}

void Timeline::schedule_in(Duration duration, Performer& performer)
{
    events.emplace(get_current_time() + duration, &performer);
}

void Timeline::schedule_at(Time time, Performer& performer)
{
    assert(time >= get_current_time());
    events.emplace(time, &performer);
}

void Timeline::wait_until(Time time)
{
    assert(time >= current_time);
    waiter.wait_for(time - current_time);
    current_time = time;
}

bool Timeline::run_step()
{
    if (events.empty())
        return false;

    auto [event_time, event_performer] = events.top(); events.pop();
    wait_until(event_time);
    event_performer->perform();
    return true;
}

void Timeline::run()
{
    bool events_left;
    while ((events_left = run_step()));
}

}
