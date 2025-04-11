#include <gtest/gtest.h>

#include "core/player.h"
#include "core/timeline.h"
#include "mock/core/player_endpoint.h"
#include "mock/core/waiter.h"
#include "mock/core/game.h"

namespace matchmaker::core::test {

using ::testing::_;
using ::testing::Return;
using ::testing::NiceMock;

class PlayerTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        ON_CALL(mock_waiter, wait_for(_)).WillByDefault(Return());
        ON_CALL(mock_game, run(_, _)).WillByDefault(Return(Game::Winner::None));
    }

    NiceMock<mock::Waiter> mock_waiter;
    NiceMock<mock::Game> mock_game;
    NiceMock<mock::PlayerEndpoint> player_endpoint;

    User user {"johndoe", "John", "Doe", {&mock_game}};
    Timeline timeline {mock_waiter, 0ms};
    Player player {user, timeline, player_endpoint};
};

TEST_F(PlayerTest, PlayerStartsFree)
{
    ASSERT_EQ(player.get_current_state(), Player::State::Free);
}

TEST_F(PlayerTest, PlayerStartsWaitingAfterBeingFree)
{
    timeline.run_step();
    EXPECT_EQ(timeline.get_current_time(), Player::free_time);
    ASSERT_EQ(player.get_current_state(), Player::State::Waiting);
}

TEST_F(PlayerTest, PlayerGoesBackToRestingAfterWaitingForTooLong)
{
    timeline.run_step();
    timeline.run_step();
    EXPECT_EQ(timeline.get_current_time(), Player::free_time + Player::wait_time);
    ASSERT_EQ(player.get_current_state(), Player::State::Free);
}

TEST_F(PlayerTest, PlayerBecomesBusyWhenPlaying)
{
    timeline.run_step();
    EXPECT_EQ(player.get_current_state(), Player::State::Waiting);

    player.play(mock_game);
    ASSERT_EQ(player.get_current_state(), Player::State::Busy);
}

TEST_F(PlayerTest, PlayerBecomesFreeAfterFinishingPlaying)
{
    timeline.run_step();
    EXPECT_EQ(player.get_current_state(), Player::State::Waiting);

    player.play(mock_game);
    EXPECT_EQ(player.get_current_state(), Player::State::Busy);

    player.finish_playing();
    ASSERT_EQ(player.get_current_state(), Player::State::Free);
}

TEST_F(PlayerTest, PlayerThrowsExceptionWhenAttemptingToPlayWhenNotWaiting)
{
    EXPECT_EQ(player.get_current_state(), Player::State::Free);
    ASSERT_THROW(player.play(mock_game), PlayerException);
}

TEST_F(PlayerTest, PlayerThrowsExceptionWhenAttemptingToFinishPlayingWhenNotBusy)
{
    EXPECT_EQ(player.get_current_state(), Player::State::Free);
    ASSERT_THROW(player.finish_playing(), PlayerException);
}

TEST_F(PlayerTest, PlayerThrowsExceptionWhenRequestingMatchWithoutHavingPrefferedGames)
{
    user = User {
        user.get_username(), std::string(user.get_name()), std::string(user.get_lastname()), {}
    };
    EXPECT_EQ(player.get_current_state(), Player::State::Free);

    ASSERT_THROW(timeline.run_step(), PlayerException);
}

}
