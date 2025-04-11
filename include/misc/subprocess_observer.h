#pragma once

namespace matchmaker::misc {

class SubprocessObserver {
public:
    virtual void notify_exit(int exit_code) = 0;
    virtual void notify_exit_fail() = 0;
};

}
