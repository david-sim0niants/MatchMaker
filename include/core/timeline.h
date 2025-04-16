#pragma once

#include "core/timing.h"
#include "core/waiter.h"

#include <functional>
#include <map>
#include <mutex>
#include <variant>

namespace matchmaker::core {

class Timeline {
    using EventID = uint64_t;

public:
    class Event {
        friend class Timeline;

    public:
        Event() = default;

        inline Time get_time() const noexcept
        {
            return time;
        }

        inline bool operator==(const Event& rhs) const noexcept
        {
            return time == rhs.time && id == rhs.id;
        }

        inline bool operator!=(const Event& rhs) const noexcept
        {
            return !(*this == rhs);
        }

    private:
        Event(Time time, EventID id) : time(time), id(id)
        {
        }

        Time time {};
        EventID id = 0;
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

    using EventCallback = std::variant<std::function<void()>, std::function<void(Event event)>>;

    explicit Timeline(Waiter& waiter, Time start_time = 0ms);

    Timeline(const Timeline&) = delete;
    Timeline& operator=(const Timeline&) = delete;

    Timeline(Timeline&&) = delete;
    Timeline& operator=(Timeline&&) = delete;

    void post(EventCallback&& event_callback);

    std::pair<Time, bool> run_once();
    std::pair<Time, bool> run_till_next_time();
    Time run();

    static bool running() noexcept;
    static Event call_in(Duration duration, EventCallback&& event_callback);
    static Event call_at(Time time, EventCallback&& event_callback);
    static void cancel(Event event);
    static void cancel_all();
    static Time get_current_time();

private:
    bool run_once_internal();
    static void call_callback(EventCallback& callback, Event& event);

    static Timeline *get_current() noexcept;
    bool is_current() const noexcept;

    Event schedule_in(Duration duration, EventCallback&& event_callback);
    Event schedule_at(Time time, EventCallback&& event_callback);
    void cancel_event(Event event);
    void cancel_all_events();

    void wait_until(Time time);
    void interrupt();

    Waiter& waiter;

    std::map<Event, EventCallback, EventCompare> events;
    std::mutex immediate_events_mutex;
    std::vector<EventCallback> immediate_events;

    Time current_time;

    static thread_local Timeline *current;
};

using EventHandle = Timeline::Event;
using EventHandleCompare = Timeline::EventCompare;

}
