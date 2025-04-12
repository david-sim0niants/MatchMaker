#pragma once

#include "core/timeline.h"

#include <map>

namespace matchmaker::core {

class TimelineObject {
protected:
    void wait_for(Duration duration);
    void wait_until(Time time);

    inline static Time get_current_time() noexcept
    {
        return Timeline::get_current_time();
    }

    void cancel_awaiting_events();

    virtual void exec() = 0;

private:
    std::map<Time, EventHandle> awaiting_events;
};

}
