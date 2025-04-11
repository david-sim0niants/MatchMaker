#include "exception.h"
#include "misc/subprocess_observer.h"

#include <sys/types.h>

namespace matchmaker::misc {

class Subprocess {
public:
    Subprocess(const char *path, char *const argv[], SubprocessObserver *observer = nullptr);
    ~Subprocess();

    Subprocess(const Subprocess&) = delete;
    Subprocess& operator=(const Subprocess&) = delete;

    Subprocess(Subprocess&&);
    Subprocess& operator=(Subprocess&&);

    void swap(Subprocess& other);
    void kill();
    int wait();

    std::string read_stdout_full() const;

    inline bool is_running() const noexcept
    {
        return running;
    }

private:
    void run(const char *path, char *const argv[]);

    pid_t pid;
    int stdout_read_fd;
    bool running;
};

class SubprocessException : public Exception {
public:
    using Exception::Exception;
    SubprocessException(std::string_view message, int error_code);

    inline int get_error_code() const noexcept
    {
        return error_code;
    }

private:
    int error_code;
};

}
