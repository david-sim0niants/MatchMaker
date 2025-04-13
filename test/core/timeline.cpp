#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "test_config.h"

#include "core/timeline.h"
#include "misc/prng.h"
#include "mock/core/waiter.h"

namespace matchmaker::core::test {

using ::testing::_;
using ::testing::NiceMock;
using ::testing::InSequence;
using ::testing::MockFunction;
using ::testing::Exactly;

class TimelineTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        ON_CALL(mock_waiter, wait_for(_)).WillByDefault(
                [](Duration duration){ return duration; }
            );
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

    mutable misc::PRNG prng {TestConfig::get_prng_seed()};
    NiceMock<mock::Waiter> mock_waiter;
    Timeline timeline {mock_waiter, 0ms};
};

TEST_F(TimelineTest, RandomlySchedulingAtSpecificTimesRunsInChronologicalOrder)
{
    MockFunction<void ()> event;
    std::array<Time, nr_events> expected_timestamps;

    std::array<std::size_t, nr_events> random_order;

    gen_timestaps(expected_timestamps);
    gen_shuffled_indices(random_order);

    InSequence seq;
    EXPECT_CALL(mock_waiter, interrupt).Times(Exactly(1));
    for (std::size_t i = 0; i < nr_events; ++i)
        EXPECT_CALL(event, Call).WillOnce(
                [this, expected_timestamp = expected_timestamps[i]]
                {
                    EXPECT_EQ(expected_timestamp, Timeline::get_current_time());
                }
            );

    timeline.sync_call(
        [&random_order, &event, &expected_timestamps]
        {
            for (std::size_t i : random_order)
                Timeline::call_at(expected_timestamps[i], event.AsStdFunction());
        }
    );
    timeline.run();
}

TEST_F(TimelineTest, SchedulingFromEventToEventAfterDurationWorks)
{
    MockFunction<void ()> event;
    Time expected_event_time = 0s;

    InSequence seq;
    EXPECT_CALL(mock_waiter, interrupt).Times(Exactly(1));

    EXPECT_CALL(event, Call).Times(nr_events - 1).WillRepeatedly(
            [&expected_event_time, &event, this]
            {
                EXPECT_EQ(expected_event_time, Timeline::get_current_time());

                Duration duration = gen_random_duration();
                expected_event_time += duration;
                Timeline::call_in(duration, event.AsStdFunction());
            }
        );
    EXPECT_CALL(event, Call).WillOnce(
            [&expected_event_time, this]
            {
                EXPECT_EQ(expected_event_time, Timeline::get_current_time());
            }
        );

    timeline.sync_call(event.AsStdFunction());

    timeline.run();
}

TEST_F(TimelineTest, ScheduleCancellationWorks)
{
    MockFunction<void ()> event;

    EXPECT_CALL(event, Call).Times(Exactly(1));

    EventHandle event_handle;
    timeline.sync_call(
            [&event, &event_handle]
            {
                event_handle = Timeline::call_at(10ms, event.AsStdFunction());
                Timeline::call_at(10ms, event.AsStdFunction());
            }
        );
    timeline.sync_call([&event, &event_handle]{ Timeline::cancel(event_handle); });
    timeline.run();
}

}
