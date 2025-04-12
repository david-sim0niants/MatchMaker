#include "core/timeline_object.h"

namespace matchmaker::core {

void TimelineObject::wait_for(Duration duration)
{
    EventHandle event = Timeline::call_in(duration,
        [this](EventHandle handle)
        {
            exec();
            // awaiting_events.erase(
            //         awaiting_events.begin(),
            //         awaiting_events.lower_bound(handle.get_time())
            //     );
        }
    );
    awaiting_events.emplace(event.get_time(), event);
}

void TimelineObject::wait_until(Time time)
{
    EventHandle event = Timeline::call_at(time,
        [this](EventHandle handle)
        {
            exec();
            // awaiting_events.erase(
            //         awaiting_events.begin(),
            //         awaiting_events.lower_bound(handle.get_time())
            //     );
        }
    );
    awaiting_events.emplace(event.get_time(), event);
}

void TimelineObject::cancel_awaiting_events()
{
    for (auto& [_, event_handle] : awaiting_events)
        Timeline::cancel(event_handle);
}

}
