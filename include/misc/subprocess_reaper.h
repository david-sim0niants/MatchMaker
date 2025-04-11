#pragma once

#include "misc/subprocess_observer.h"

#include <atomic>
#include <mutex>
#include <thread>
#include <unordered_map>

namespace matchmaker::misc {

class SubprocessReaper {
public:
    SubprocessReaper(const SubprocessReaper&) = delete;
    SubprocessReaper& operator=(const SubprocessReaper&) = delete;

    SubprocessReaper(SubprocessReaper&&) = delete;
    SubprocessReaper& operator=(SubprocessReaper&&) = delete;

    static inline SubprocessReaper& get_instance()
    {
        return instance;
    }

    void set_observer_of(pid_t pid, SubprocessObserver& observer);
    void unset_observer_of(pid_t pid);

private:
    SubprocessReaper();
    ~SubprocessReaper();

    void notify_exit(pid_t pid, int exit_code);
    void notify_exit_fail(pid_t pid);
    SubprocessObserver *takeoff_observer_of(pid_t pid);

    void reap();
    void stop();

    std::mutex mutex;
    std::unordered_map<pid_t, SubprocessObserver *> observer_by_pid;
    std::atomic_bool running;
    std::thread thread;

    static SubprocessReaper instance;
};

inline SubprocessReaper SubprocessReaper::instance;

}
