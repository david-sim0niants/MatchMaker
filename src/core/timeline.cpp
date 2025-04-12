#include "core/timeline.h"
#include "defer.h"

#include <cassert>

namespace matchmaker::core {

Timeline::Timeline(Waiter& waiter, Time start_time) :
    waiter(waiter), current_time(start_time)
{
}

Timeline::~Timeline()
{
    if (is_current())
        unset_as_current();
}

void Timeline::join(EventCallback&& event_callback)
{
    std::scoped_lock lock {async_immediate_events_mutex};
    async_immediate_events.emplace_back(std::move(event_callback));
    interrupt();
}

std::pair<Time, bool> Timeline::run_once()
{
    return std::make_pair(current_time, run_once_internal());
}

Time Timeline::run()
{
    bool needs_to_run;
    while ((needs_to_run = run_once_internal()));
    return current_time;
}

bool Timeline::running() noexcept
{
    return get_current() != nullptr;
}

EventHandle Timeline::call_in(Duration duration, EventCallback&& event_callback)
{
    assert(running() && "No timeline currently running.");
    return get_current()->schedule_in(duration, std::move(event_callback));
}

EventHandle Timeline::call_at(Time time, EventCallback&& event_callback)
{
    assert(running() && "No timeline currently running.");
    return get_current()->schedule_at(time, std::move(event_callback));
}

void Timeline::cancel(EventHandle event_handle)
{
    assert(running() && "No timeline currently running.");
    return get_current()->cancel_event(event_handle);
}

Time Timeline::get_current_time()
{
    assert(running() && "No timeline currently running.");
    return get_current()->current_time;
}

bool Timeline::run_once_internal()
{
    set_as_current();
    DEFER (unset_as_current());

    {
        std::scoped_lock lock {async_immediate_events_mutex};
        for (EventCallback& async_event : async_immediate_events)
            schedule_at(current_time, std::move(async_event));
        async_immediate_events.clear();
    }

    if (events.empty())
        return false;

    Time event_time = events.begin()->first.time;
    wait_until(event_time);

    if (current_time >= event_time) {
        EventCallback& event_callback = events.begin()->second;
        event_callback();
        events.erase(events.begin());
        current_time = event_time;
    }

    return true;
}

Timeline *Timeline::get_current() noexcept
{
    return nested_timelines.empty() ? nullptr : nested_timelines.top();
}

bool Timeline::is_current() const noexcept
{
    return !nested_timelines.empty() && nested_timelines.top() == this;
}

void Timeline::set_as_current()
{
    nested_timelines.push(this);
}

void Timeline::unset_as_current()
{
    nested_timelines.pop();
}

EventHandle Timeline::schedule_in(Duration duration, EventCallback&& event_callback)
{
    return schedule_at(current_time + duration, std::move(event_callback));
}

EventHandle Timeline::schedule_at(Time time, EventCallback&& event_callback)
{
    assert(time >= current_time);

    EventHandle event_handle;
    event_handle.event = { .time = time, .id = 0};

    auto event_it = events.upper_bound(time);
    if (event_it != events.begin() && (--event_it)->first.time == time) {
        ++event_it;
        event_handle.event.id = event_it->first.id + 1;
    }
    events.emplace_hint(event_it, event_handle.event, std::move(event_callback));
    return event_handle;
}

void Timeline::cancel_event(EventHandle event_handle)
{
    events.erase(event_handle.event);
}

void Timeline::wait_until(Time time)
{
    assert(time >= current_time);
    if (time == current_time)
        return;
    Duration time_passed = waiter.wait_for(time - current_time);
    current_time += time_passed;
}

void Timeline::interrupt()
{
    waiter.interrupt();
}

thread_local std::stack<Timeline *> Timeline::nested_timelines;

}
