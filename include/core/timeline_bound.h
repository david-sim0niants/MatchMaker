#pragma once

#include "core/performer.h"
#include "core/timeline.h"

namespace matchmaker::core {

class TimelineBound : public Performer {
public:
    explicit TimelineBound(Timeline& timeline) :
        timeline(timeline)
    {
    }

    void wait_for(Duration duration);
    void wait_until(Time time);

protected:
    inline Time get_current_time() const noexcept
    {
        return timeline.get_current_time();
    }

    Timeline& timeline;
};

}
