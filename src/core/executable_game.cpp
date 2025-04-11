#include "core/executable_game.h"
#include "core/game_instance.h"
#include "core/player.h"
#include "misc/subprocess.h"

#include <algorithm>
#include <memory>
#include <sys/wait.h>

namespace matchmaker::core {

static std::pair<std::string_view, std::string_view>
    find_last_two_words(std::string_view str)
{
    auto is_space = [](char ch) { return std::isspace(static_cast<unsigned char>(ch)); };

    auto r = str.rbegin();
    auto rend = str.rend();

    r = std::find_if_not(r, rend, is_space);

    auto word2_end = r;
    auto word2_start = std::find_if(word2_end, rend, is_space);

    auto word1_end = std::find_if_not(word2_start, rend, is_space);
    auto word1_start = std::find_if(word1_end, rend, is_space);

    std::string_view w1(&*word1_start, word1_end - word1_start);
    std::string_view w2(&*word2_start, word2_end - word2_start);

    return {w1, w2};
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
        subprocess(make_subprocess(exec_path, exec_name, player_a_name, player_b_name, this)),
        observer(observer)
    {
    }

    ~ExecutableGameInstance()
    {
        stop();
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

ExecutableGame::ExecutableGame(std::string&& name, std::string&& path)
    : name(std::move(name)), path(std::move(path))
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
            path.c_str(), name.c_str(), player_a_name, player_b_name, observer);
}

}
