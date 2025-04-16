#include "core/executable_game.h"
#include "core/game_instance.h"
#include "core/player.h"
#include "misc/subprocess.h"

#include <iostream>
#include <memory>
#include <sys/wait.h>

namespace matchmaker::core {

namespace {

static std::pair<std::string_view, std::string_view> find_last_two_words(std::string_view str)
{
    // Trim trailing whitespace
    size_t end = str.size();
    while (end > 0 && std::isspace(str[end - 1])) {
        --end;
    }

    if (end == 0) {
        return { {}, {} };
    }

    // Find the last word
    size_t last_word_end = end - 1;
    size_t last_word_start = last_word_end;

    while (last_word_start > 0 && !std::isspace(str[last_word_start - 1])) {
        --last_word_start;
    }

    std::string_view last = str.substr(last_word_start, last_word_end - last_word_start + 1);

    if (last_word_start == 0) {
        return { {}, last };
    }

    // Skip whitespace between the two words
    size_t second_end = last_word_start - 1;
    while (second_end > 0 && std::isspace(str[second_end])) {
        --second_end;
    }

    if (second_end == 0 && std::isspace(str[second_end])) {
        return { {}, last };
    }

    // Find the second last word
    size_t second_word_start = second_end;
    while (second_word_start > 0 && !std::isspace(str[second_word_start - 1])) {
        --second_word_start;
    }

    std::string_view second_last = str.substr(second_word_start, second_end - second_word_start + 1);

    return { second_last, last };
}

static misc::Subprocess make_subprocess(
        const char *exec_path, const char *exec_name,
        const char *player_a_name, const char *player_b_name,
        misc::SubprocessObserver *observer)
{
    char *argv[] = {
        const_cast<char *>(exec_name),
        const_cast<char *>(player_a_name),
        const_cast<char *>(player_b_name),
        NULL
    };
    return misc::Subprocess(exec_path, argv, observer);
}

class ExecutableGameInstance : public GameInstance, private misc::SubprocessObserver {
public:
    ExecutableGameInstance(
            const char *exec_path, const char *exec_name,
            const char *player_a_name, const char *player_b_name,
            GameInstanceObserver *observer) :
        player_a_name(player_a_name), player_b_name(player_b_name),
        observer(observer),
        subprocess(make_subprocess(exec_path, exec_name, player_a_name, player_b_name, this))
    {
    }

    virtual void stop() override
    {
        subprocess.kill();
    }

    virtual GameWinner wait_and_get() override
    {
        subprocess.wait();
        return parse_result();
    }

private:
    void notify_exit(int exit_code) override
    {
        if (observer)
            observer->notify_finished(parse_result());
    }

    void notify_exit_fail() override
    {
        // throw ExecutableGameException("subprocess exit failure");
    }

    GameWinner parse_result() const
    {
        std::string output = subprocess.read_stdout_full();
        std::cout << output << std::endl;
        auto [word1, word2] = find_last_two_words(output);
        if (word1 == player_a_name && word2 == player_b_name ||
            word1 == player_b_name && word2 == player_a_name)
            return GameWinner::None;
        else if (word2 == player_a_name)
            return GameWinner::PlayerA;
        else if (word2 == player_b_name)
            return GameWinner::PlayerB;
        else
            throw ExecutableGameException("invalid output from the executable");
    }

    const char *player_a_name, *player_b_name;
    misc::Subprocess subprocess;
    GameInstanceObserver *observer;
};

}

ExecutableGame::ExecutableGame(std::string&& name, std::string&& comm, std::string&& path)
    : Game(std::move(name)), comm(std::move(comm)), path(std::move(path))
{
}

std::unique_ptr<GameInstance> ExecutableGame::launch(
        const Player& player_a,
        const Player& player_b,
        GameInstanceObserver *observer) const
{
    const char *player_a_name = player_a.get_user().get_username();
    const char *player_b_name = player_b.get_user().get_username();

    return std::make_unique<ExecutableGameInstance>(
            path.c_str(), comm.c_str(), player_a_name, player_b_name, observer);
}

}
