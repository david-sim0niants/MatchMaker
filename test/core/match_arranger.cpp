#include <gtest/gtest.h>

#include "core/match_arranger.h"
#include "core/player.h"
#include "core/timeline.h"
#include "mock/core/game.h"
#include "mock/core/waiter.h"

namespace matchmaker::core::test {

class MatchArrangerTest : public ::testing::Test {
protected:
    mock::Game game;
    User user_a {"alice", "Alice", "Cooper", {&game}};
    User user_b {"bob", "Bob", "Dylan", {&game}};
    mock::Waiter waiter;
    Timeline timeline {waiter};
    Player player_a {user_a, timeline}, player_b {user_b, timeline};
    MatchArranger match_arranger;
};

TEST_F(MatchArrangerTest, RequestingMatchCreatesAnInactiveMatchAndReturnsNothing)
{
    ASSERT_FALSE(match_arranger.find_or_request_match(player_a, 0, game).has_value());
    ASSERT_TRUE(match_arranger.has_arranged_match_for(player_a));
}

TEST_F(MatchArrangerTest, SamePlayerRequestingMatchTwiceThrowsException)
{
    match_arranger.find_or_request_match(player_a, 0, game);
    ASSERT_THROW(match_arranger.find_or_request_match(player_a, 0, game), MatchArrangerException);
}

TEST_F(MatchArrangerTest, HasArrangedMatchForMethodReturnsFalseForPlayerThatDidNotRequestMatch)
{
    ASSERT_FALSE(match_arranger.has_arranged_match_for(player_a));
}

TEST_F(MatchArrangerTest, ArrangingMatchForTwoPlayersWithTheSameGameWorksAsExpected)
{
    EXPECT_FALSE(match_arranger.find_or_request_match(player_a, 4, game).has_value());
    auto opt_match = match_arranger.find_or_request_match(player_b, 4, game);
    ASSERT_TRUE(opt_match.has_value());

    auto& match = *opt_match;

    EXPECT_TRUE(match.both_players_present());
    EXPECT_EQ(match.get_player_a(), &player_a);
    EXPECT_EQ(match.get_player_b(), &player_b);
}

TEST_F(MatchArrangerTest, ReleasingMatchEnsuresBothPlayersHaveNoMatchesLeftArranged)
{
    EXPECT_FALSE(match_arranger.find_or_request_match(player_a, 4, game).has_value());
    EXPECT_TRUE(match_arranger.find_or_request_match(player_b, 4, game).has_value());

    ASSERT_FALSE(match_arranger.has_arranged_match_for(player_a));
    ASSERT_FALSE(match_arranger.has_arranged_match_for(player_b));
}

}
