#include "core/timeline_object.h"

namespace matchmaker::core {

void TimelineObject::wait_for(Duration duration)
{
    EventHandle event = Timeline::call_in(duration,
        [this](EventHandle handle)
        {
            exec();
            awaiting_events.erase(handle);
        }
    );
    awaiting_events.emplace(event);
}

void TimelineObject::wait_until(Time time)
{
    EventHandle event = Timeline::call_at(time,
        [this](EventHandle handle)
        {
            exec();
            awaiting_events.erase(handle);
        }
    );
    awaiting_events.emplace(event);
}

void TimelineObject::cancel_awaiting_events()
{
    for (auto&& event : awaiting_events)
        Timeline::cancel(event);
}

}
