#include "core/timeline_bound.h"

namespace matchmaker::core {

void TimelineBound::wait_for(Duration duration)
{
    timeline.schedule_in(duration, *this);
}

void TimelineBound::wait_until(Time time)
{
    timeline.schedule_at(time, *this);
}

}
