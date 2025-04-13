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

void Timeline::sync_call(EventCallback&& event_callback)
{
    std::scoped_lock lock {async_immediate_events_mutex};
    async_immediate_events.emplace_back(std::move(event_callback));
    interrupt();
}

std::pair<Time, bool> Timeline::run_once()
{
    return std::make_pair(current_time, run_once_internal());
}

std::pair<Time, bool> Timeline::run_till_next_time()
{
    Time prev_time = current_time;
    bool needs_to_run;
    do {
        needs_to_run = run_once_internal();
    } while (prev_time == current_time && needs_to_run);
    return std::make_pair(current_time, needs_to_run);
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

void Timeline::cancel(Event event)
{
    assert(running() && "No timeline currently running.");
    return get_current()->cancel_event(event);
}

void Timeline::cancel_all()
{
    assert(running() && "No timeline currently running.");
    return get_current()->cancel_all_events();
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
        current_time = event_time;

        Event event{events.begin()->first};
        EventCallback& event_callback = events.begin()->second;
        call_callback(event_callback, event);

        events.erase(events.begin());
    }

    return true;
}

void Timeline::call_callback(EventCallback& callback, Event& handle)
{
    struct Visitor {
        void operator()(std::function<void()>& f)
        {
            f();
        }

        void operator()(std::function<void(Event)>& f)
        {
            f(handle);
        }

        Event& handle;
    };
    std::visit(Visitor{handle}, callback);
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

Timeline::Event Timeline::schedule_in(Duration duration, EventCallback&& event_callback)
{
    return schedule_at(current_time + duration, std::move(event_callback));
}

Timeline::Event Timeline::schedule_at(Time time, EventCallback&& event_callback)
{
    assert(time >= current_time);

    Event event {time, 0};

    auto event_it = events.upper_bound(time);
    if (event_it != events.begin() && (--event_it)->first.time == time) {
        event.id = event_it->first.id + 1;
        ++event_it;
    }
    events.emplace_hint(event_it, event, std::move(event_callback));
    return event;
}

void Timeline::cancel_event(Event event)
{
    auto event_it = events.find(event);
    if (event_it != events.end())
        events.erase(event_it);
}

void Timeline::cancel_all_events()
{
    events.clear();
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
