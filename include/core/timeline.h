#pragma once

#include "core/timing.h"
#include "core/waiter.h"

#include <functional>
#include <map>
#include <mutex>
#include <stack>

namespace matchmaker::core {

class Timeline {
    using EventID = uint64_t;

    struct Event {
        Time time;
        EventID id;
    };

    struct EventCompare {
        using is_transparent = void;

        bool operator()(const Event& lhs, const Event& rhs) const noexcept
        {
            return std::tie(lhs.time, lhs.id) < std::tie(rhs.time, rhs.id);
        }

        bool operator()(const Event& lhs, Time rhs) const noexcept
        {
            return lhs.time < rhs;
        }

        bool operator()(Time lhs, const Event& rhs) const noexcept
        {
            return lhs < rhs.time;
        }
    };

public:
    using EventCallback = std::function<void()>;

    class EventHandle {
        friend class Timeline;
        Event event;
    };

    explicit Timeline(Waiter& waiter, Time start_time = 0ms);
    ~Timeline();

    Timeline(const Timeline&) = delete;
    Timeline& operator=(const Timeline&) = delete;

    Timeline(Timeline&&) = delete;
    Timeline& operator=(Timeline&&) = delete;

    void join(EventCallback&& event_callback);

    std::pair<Time, bool> run_once();
    Time run();

    static bool running() noexcept;
    static EventHandle call_in(Duration duration, EventCallback&& event_callback);
    static EventHandle call_at(Time time, EventCallback&& event_callback);
    static void cancel(EventHandle event_handle);
    static Time get_current_time();

private:
    bool run_once_internal();

    static Timeline *get_current() noexcept;
    bool is_current() const noexcept;
    void set_as_current();
    void unset_as_current();

    EventHandle schedule_in(Duration duration, EventCallback&& event_callback);
    EventHandle schedule_at(Time time, EventCallback&& event_callback);
    void cancel_event(EventHandle event_handle);

    void wait_until(Time time);
    void interrupt();

    Waiter& waiter;

    std::map<Event, EventCallback, EventCompare> events;
    std::mutex async_immediate_events_mutex;
    std::vector<EventCallback> async_immediate_events;

    Time current_time;

    static thread_local std::stack<Timeline *> nested_timelines;
};

using EventHandle = Timeline::EventHandle;

}
