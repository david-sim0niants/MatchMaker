#pragma once

namespace matchmaker::core {

enum class GameWinner {
    None, PlayerA, PlayerB
};

class GameInstanceObserver {
public:
    virtual void notify_finished(GameWinner winner) = 0;
};

class GameInstance {
public:
    explicit GameInstance(GameInstanceObserver *observer = nullptr) :
        observer(observer)
    {
    }

    virtual ~GameInstance() = default;

    virtual void stop() = 0;
    virtual GameWinner wait_and_get() = 0;

private:
    GameInstanceObserver *observer = nullptr;
};

}
