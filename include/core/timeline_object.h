#pragma once

#include "core/timeline.h"

namespace matchmaker::core {

class TimelineObject {
protected:
    void wait_for(Duration duration);
    void wait_until(Time time);

    inline static Time get_current_time() noexcept
    {
        return Timeline::get_current_time();
    }

    virtual void exec() = 0;
};

}
