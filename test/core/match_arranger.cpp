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
    mock::Waiter waiter;
    Timeline timeline {waiter};

    User user_1 {"alice", "Alice", "Cooper", {&game}};
    User user_2 {"bob", "Bob", "Dylan", {&game}};
    User user_3 {"johndoe", "John", "Doe", {&game}};

    Player player_1 {user_1, timeline};
    Player player_2 {user_2, timeline};
    Player player_3 {user_3, timeline};

    MatchArranger match_arranger;
};

TEST_F(MatchArrangerTest, RequestingMatchCreatesAnInactiveMatchAndReturnsNothing)
{
    ASSERT_FALSE(match_arranger.find_or_request_match(player_1, 0, game));
    ASSERT_TRUE(match_arranger.has_arranged_match_for(player_1));
}

TEST_F(MatchArrangerTest, SamePlayerRequestingMatchTwiceThrowsException)
{
    match_arranger.find_or_request_match(player_1, 0, game);
    ASSERT_THROW(match_arranger.find_or_request_match(player_1, 0, game), MatchArrangerException);
}

TEST_F(MatchArrangerTest, HasArrangedMatchForMethodReturnsFalseForPlayerThatDidNotRequestMatch)
{
    ASSERT_FALSE(match_arranger.has_arranged_match_for(player_1));
}

TEST_F(MatchArrangerTest, ArrangingMatchForTwoPlayersWithTheSameGameWorksAsExpected)
{
    EXPECT_FALSE(match_arranger.find_or_request_match(player_1, 4, game));
    auto match = match_arranger.find_or_request_match(player_2, 4, game);
    ASSERT_TRUE(match);

    EXPECT_TRUE(match->both_players_present());
    EXPECT_EQ(match->get_player_a(), &player_1);
    EXPECT_EQ(match->get_player_b(), &player_2);
}

TEST_F(MatchArrangerTest, ReleasingMatchEnsuresBothPlayersHaveNoMatchesLeftArranged)
{
    EXPECT_FALSE(match_arranger.find_or_request_match(player_1, 4, game));
    EXPECT_TRUE(match_arranger.find_or_request_match(player_2, 4, game));

    ASSERT_FALSE(match_arranger.has_arranged_match_for(player_1));
    ASSERT_FALSE(match_arranger.has_arranged_match_for(player_2));
}

TEST_F(MatchArrangerTest, RequestingMatchAndThenWithdrawingWorksAsExpected)
{
    match_arranger.find_or_request_match(player_1, 0, game);
    EXPECT_TRUE(match_arranger.has_arranged_match_for(player_1));

    match_arranger.withdraw_match(player_1);
    ASSERT_FALSE(match_arranger.has_arranged_match_for(player_1));
}

TEST_F(MatchArrangerTest, TooWeakPlayerIsNotMatchedWithTooStrongPlayer)
{
    const int weak_rating = 0;
    const int strong_rating = 1 +
        MatchArranger::lower_rating_diff_thresh +
        MatchArranger::higher_rating_diff_thresh;

    EXPECT_FALSE(match_arranger.find_or_request_match(player_1, 0, game));
    ASSERT_FALSE(match_arranger.find_or_request_match(player_2, strong_rating, game));

    ASSERT_TRUE(match_arranger.has_arranged_match_for(player_1));
    ASSERT_TRUE(match_arranger.has_arranged_match_for(player_2));
}

TEST_F(MatchArrangerTest, FairestMatchIsTaken1)
{
    const int weak_rating = 0;
    const int strong_rating = 1 +
        MatchArranger::lower_rating_diff_thresh +
        MatchArranger::higher_rating_diff_thresh;
    const int mid_rating = strong_rating - MatchArranger::higher_rating_diff_thresh;

    EXPECT_FALSE(match_arranger.find_or_request_match(player_1, weak_rating, game));
    EXPECT_FALSE(match_arranger.find_or_request_match(player_2, strong_rating, game));

    auto match = match_arranger.find_or_request_match(player_3, mid_rating, game);
    ASSERT_TRUE(match);

    EXPECT_EQ(match->get_player_b(), &player_3);
    ASSERT_EQ(match->get_player_a(), &player_2);
}

TEST_F(MatchArrangerTest, FairestMatchIsTaken2)
{
    const int weak_rating = 0;
    const int strong_rating = 1 +
        MatchArranger::lower_rating_diff_thresh +
        MatchArranger::higher_rating_diff_thresh;
    const int mid_rating = weak_rating + MatchArranger::lower_rating_diff_thresh;

    EXPECT_FALSE(match_arranger.find_or_request_match(player_1, weak_rating, game));
    EXPECT_FALSE(match_arranger.find_or_request_match(player_2, strong_rating, game));

    auto match = match_arranger.find_or_request_match(player_3, mid_rating, game);
    ASSERT_TRUE(match);

    EXPECT_EQ(match->get_player_b(), &player_3);
    ASSERT_EQ(match->get_player_a(), &player_1);
}

}
