#include <gtest/gtest.h>

#include "core/player.h"
#include "core/timeline.h"
#include "core/match.h"
#include "misc/prng.h"
#include "mock/core/player_endpoint.h"
#include "mock/core/waiter.h"
#include "mock/core/game.h"

namespace matchmaker::core::test {

using ::testing::_;
using ::testing::NiceMock;

class PlayerTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        ON_CALL(mock_waiter, wait_for(_)).WillByDefault(
                [](Duration duration){ return duration; }
            );
        timeline.post([this]{ player.init(); });
        timeline.run_once();
    }

    misc::PRNG prng;

    NiceMock<mock::Waiter> mock_waiter;
    NiceMock<mock::Game> mock_game;
    NiceMock<mock::PlayerEndpoint> player_endpoint;
    Match mock_match {mock_game};

    User user {"johndoe", "John", "Doe", {&mock_game}};
    Timeline timeline {mock_waiter, 0ms};

    Player player {user, player_endpoint, prng};
};

TEST_F(PlayerTest, PlayerStartsFree)
{
    ASSERT_EQ(player.get_current_state(), Player::State::Free);
}

TEST_F(PlayerTest, PlayerStartsWaitingAfterBeingFree)
{
    auto [curr_time, needs_run] = timeline.run_once();
    EXPECT_EQ(curr_time.count(), Player::free_time.count());
    ASSERT_EQ(player.get_current_state(), Player::State::Waiting);
}

TEST_F(PlayerTest, PlayerGoesBackToRestingAfterWaitingForTooLong)
{
    timeline.run_once();
    auto [curr_time, needs_run] = timeline.run_once();
    EXPECT_GE(curr_time.count(), (Player::free_time + Player::min_wait_time).count());
    EXPECT_LE(curr_time.count(), (Player::free_time + Player::max_wait_time).count());
    ASSERT_EQ(player.get_current_state(), Player::State::Free);
}

TEST_F(PlayerTest, PlayerBecomesBusyWhenPlaying)
{
    timeline.run_once();
    EXPECT_EQ(player.get_current_state(), Player::State::Waiting);

    timeline.post([&]{ player.play(mock_match); });
    timeline.run_once();
    ASSERT_EQ(player.get_current_state(), Player::State::Busy);
}

TEST_F(PlayerTest, PlayerBecomesFreeAfterFinishingPlaying)
{
    timeline.run_once();
    EXPECT_EQ(player.get_current_state(), Player::State::Waiting);

    timeline.post([&]{ player.play(mock_match); });
    timeline.run_once();
    EXPECT_EQ(player.get_current_state(), Player::State::Busy);

    timeline.post([&]{ player.finish_playing(); });
    timeline.run_once();
    ASSERT_EQ(player.get_current_state(), Player::State::Free);
}

TEST_F(PlayerTest, PlayerThrowsExceptionWhenAttemptingToPlayWhenNotWaiting)
{
    EXPECT_EQ(player.get_current_state(), Player::State::Free);
    timeline.post([&]{ player.play(mock_match); });
    ASSERT_THROW(timeline.run_once(), PlayerException);
}

TEST_F(PlayerTest, PlayerThrowsExceptionWhenAttemptingToFinishPlayingWhenNotBusy)
{
    EXPECT_EQ(player.get_current_state(), Player::State::Free);
    timeline.post([&]{ player.finish_playing(); });
    ASSERT_THROW(timeline.run_once(), PlayerException);
}

TEST_F(PlayerTest, PlayerThrowsExceptionWhenRequestingMatchWithoutHavingPrefferedGames)
{
    user = User {
        user.get_username(), std::string(user.get_name()), std::string(user.get_lastname()), {}
    };
    EXPECT_EQ(player.get_current_state(), Player::State::Free);

    ASSERT_THROW(timeline.run_once(), PlayerException);
}

}
