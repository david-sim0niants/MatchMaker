#pragma once

#include "core/player.h"

namespace matchmaker::app {

class ConsolePlayerObserver : public core::PlayerObserver {
public:
    void on_state_change(const core::Player& player, core::Player::State state, core::Time time)
        override;

    void on_match_start(const core::Player& player, core::Match& match, core::Time time)
        override;

    void on_match_leave(const core::Player& player, core::Time time) override;
};

}
