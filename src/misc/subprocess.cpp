#include "misc/subprocess.h"
#include "misc/subprocess_reaper.h"
#include "misc/printing.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/wait.h>

namespace matchmaker::misc {

static std::pair<int, int> open_pipe()
{
    int pipe_fds[2];
    if (pipe(pipe_fds) == -1)
        throw SubprocessException("pipe() failed", errno);
    return std::make_pair(pipe_fds[0], pipe_fds[1]);
}

static void run_child(int pipe_read, int pipe_write, const char *path, char *const argv[])
{
    close(pipe_read);

    if (dup2(pipe_write, STDOUT_FILENO) < 0)
        throw SubprocessException("dup2() failed", errno);

    close(pipe_write);
    if ( execv(path, argv) < 0)
        throw SubprocessException("excev() failed", errno);

    assert(false && "Must not reach here!");
}

std::string read_full(int fd)
{
    char buffer[BUFSIZ];
    std::string result;

    while (true) {
        ssize_t bytes_read = read(fd, buffer, BUFSIZ);
        if (bytes_read == -1) {
            if (errno == EINTR)
                continue;
            throw SubprocessException("read() failed", errno);
        }
        if (bytes_read == 0)
            break; // EOF
        result.append(buffer, bytes_read);
    }

    return result;
}

Subprocess::Subprocess(const char *path, char *const argv[], SubprocessObserver *observer)
{
    run(path, argv);
    if (observer)
        SubprocessReaper::get_instance().set_observer_of(pid, *observer);
}

Subprocess::~Subprocess()
{
    if (running)
        kill();
    close(stdout_read_fd);
}

Subprocess::Subprocess(Subprocess&& other)
{
    swap(other);
}

Subprocess& Subprocess::operator=(Subprocess&& rhs)
{
    if (this != &rhs)
        swap(rhs);
    return *this;
}

void Subprocess::swap(Subprocess& other)
{
    std::swap(pid, other.pid);
    std::swap(stdout_read_fd, other.stdout_read_fd);
    std::swap(running, other.running);
}

void Subprocess::run(const char *path, char *const argv[])
{
    auto [pipe_read, pipe_write] = open_pipe();
    pid_t pid = fork();
    if (pid < 0)
        throw SubprocessException("fork() failed", errno);

    if (0 == pid)
        run_child(pipe_read, pipe_write, path, argv);
    else
        close(pipe_write);

    this->pid = pid;
    stdout_read_fd = pipe_read;
    running = true;
}

void Subprocess::kill()
{
    if (! running)
        throw SubprocessException("subprocess is already dead", errno);
    if (::kill(pid, SIGKILL) < 0)
        throw SubprocessException("kill() failed", errno);
    running = false;
}

int Subprocess::wait()
{
    int status;
    if (waitpid(pid, &status, 0) < 0)
        throw SubprocessException("waitpid() failed", errno);

    running = false;
    if (WIFEXITED(status))
        return WEXITSTATUS(status);
    else
        return -1;
}

std::string Subprocess::read_stdout_full() const
{
    return read_full(stdout_read_fd);
}

SubprocessException::SubprocessException(std::string_view message, int error_code)
    : Exception(misc::stringify(message, ": ", std::strerror(error_code)))
{
}

}
