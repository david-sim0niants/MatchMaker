#include "misc/subprocess_reaper.h"
#include "misc/printing.h"

#include <cstring>
#include <sys/wait.h>

namespace matchmaker::misc {

SubprocessReaper::SubprocessReaper() :
    running(true), thread(&SubprocessReaper::reap, this)
{
}

SubprocessReaper::~SubprocessReaper()
{
    stop();
    if (thread.joinable())
        thread.join();
}

void SubprocessReaper::set_observer_of(pid_t pid, SubprocessObserver& observer)
{
    std::scoped_lock lock {mutex};
    observer_by_pid[pid] = &observer;
    if (observer_by_pid.size() == 1)
        cv.notify_one();
}

void SubprocessReaper::unset_observer_of(pid_t pid)
{
    std::scoped_lock lock {mutex};
    observer_by_pid.erase(pid);
}

void SubprocessReaper::notify_exit(pid_t pid, int exit_code)
{
    SubprocessObserver *observer = takeoff_observer_of(pid);
    if (observer)
        observer->notify_exit(exit_code);
}

void SubprocessReaper::notify_exit_fail(pid_t pid)
{
    SubprocessObserver *observer = takeoff_observer_of(pid);
    if (observer)
        observer->notify_exit_fail();
}

SubprocessObserver *SubprocessReaper::takeoff_observer_of(pid_t pid)
{
    std::scoped_lock lock {mutex};
    auto observer_it = observer_by_pid.find(pid);
    if (observer_it == observer_by_pid.end())
        return nullptr;

    SubprocessObserver *observer = observer_it->second;
    observer_by_pid.erase(observer_it);
    return observer;
}

void SubprocessReaper::reap()
{
    while (running.load(std::memory_order_acquire)) {
        {
            std::unique_lock lock {mutex};
            cv.wait(lock, [this]{ return ! observer_by_pid.empty() ||
                    !running.load(std::memory_order_relaxed); });
        }
        if (!running.load(std::memory_order_acquire))
            break;

        int status;
        pid_t pid = waitpid(-1, &status, 0);
        if (pid > 0) {
            if (WIFEXITED(status))
                notify_exit(pid, WEXITSTATUS(status));
            else
                notify_exit_fail(pid);
        } else {
            throw std::runtime_error(misc::stringify("waitpid() failed: ", strerror(errno)));
        }
    }
}

void SubprocessReaper::stop()
{
    std::scoped_lock lock {mutex};
    running.store(false, std::memory_order_release);
    observer_by_pid.clear();
    cv.notify_one();
}

}
