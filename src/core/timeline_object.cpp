#include "core/timeline_object.h"

namespace matchmaker::core {

void TimelineObject::wait_for(Duration duration)
{
    Timeline::call_in(duration, [this]() { exec(); });
}

void TimelineObject::wait_until(Time time)
{
    Timeline::call_at(time, [this]() { exec(); });
}

}
