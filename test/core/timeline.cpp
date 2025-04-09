#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "test_config.h"

#include "core/timeline.h"
#include "misc/prng.h"
#include "mock/core/performer.h"
#include "mock/core/waiter.h"

namespace matchmaker::core::test {

using ::testing::_;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::InSequence;

class TimelineTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        ON_CALL(mock_waiter, wait_for(_)).WillByDefault(Return());
    }

    static constexpr std::size_t nr_events = 16;

    inline Duration gen_random_duration() const
    {
        return Duration(prng(0, 10000));
    }

    template<std::size_t N>
    void gen_timestaps(std::array<Time, N>& timestamps)
    {
        Time prev_time = 0s;
        for (Time& time : timestamps) {
            time = prev_time + gen_random_duration();
            prev_time = time;
        }
    }

    template<std::size_t N>
    void gen_shuffled_indices(std::array<std::size_t, N>& indices)
    {
        for (std::size_t i = 0; i < indices.size(); ++i)
            indices[i] = i;
        std::shuffle(indices.begin(), indices.end(), prng.get_generator());
    }

    misc::PRNG prng {TestConfig::get_prng_seed()};
    NiceMock<mock::Waiter> mock_waiter;
    Timeline timeline {mock_waiter, 0ms};
};

TEST_F(TimelineTest, RandomlySchedulingAtSpecificTimesRunsInChronologicalOrder)
{
    std::array<mock::Performer, nr_events> performers;
    std::array<Time, nr_events> expected_timestamps;

    std::array<std::size_t, nr_events> random_order;

    gen_timestaps(expected_timestamps);
    gen_shuffled_indices(random_order);

    InSequence seq;
    for (std::size_t i = 0; i < nr_events; ++i)
        EXPECT_CALL(performers[i], perform).WillOnce(
                [this, &expected_timestamp = expected_timestamps[i]]
                {
                    EXPECT_EQ(timeline.get_current_time(), expected_timestamp);
                }
            );

    for (std::size_t i : random_order)
        timeline.schedule_at(expected_timestamps[i], performers[i]);

    timeline.run();
}

TEST_F(TimelineTest, SchedulingFromEventToEventAfterDurationWorks)
{
    mock::Performer performer;
    Time expected_event_time = 0s;

    InSequence seq;
    EXPECT_CALL(performer, perform).Times(nr_events - 1).WillRepeatedly(
            [&performer, &expected_event_time, this]
            {
                EXPECT_EQ(expected_event_time, timeline.get_current_time());

                Duration duration = gen_random_duration();
                expected_event_time += duration;
                timeline.schedule_in(duration, performer);
            }
        );
    EXPECT_CALL(performer, perform).WillOnce(
            [&expected_event_time, this]
            {
                EXPECT_EQ(expected_event_time, timeline.get_current_time());
            }
        );

    timeline.schedule_at(expected_event_time, performer);
    timeline.run();
}

}
