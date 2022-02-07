#include "pch.h"

namespace UnitTests {
    class TimeTests : public ::testing::Test
    {
    protected:
        DeseaseSpreadSimulation::TimeManager time;
        std::vector<DeseaseSpreadSimulation::Day> weekdays{
            DeseaseSpreadSimulation::Day::Monday,
            DeseaseSpreadSimulation::Day::Tuesday,
            DeseaseSpreadSimulation::Day::Wednesday,
            DeseaseSpreadSimulation::Day::Thursday,
            DeseaseSpreadSimulation::Day::Friday,
            DeseaseSpreadSimulation::Day::Saturday,
            DeseaseSpreadSimulation::Day::Sunday
        };
    };
    TEST_F(TimeTests, HourProgessTest)
    {
        for (size_t i = 0; i < 100; i++)
        {
            EXPECT_EQ(time.GetElapsedHours(), i);
            time.Update();
        }
    }
    TEST_F(TimeTests, DayProgressTest)
    {
        EXPECT_EQ(time.GetElapsedDays(), 0);

        for (size_t i = 0; i < 24; i++)
        {
            EXPECT_EQ(time.GetElapsedDays(), 0);
            time.Update();
        }
        EXPECT_EQ(time.GetElapsedDays(), 1);

        for (size_t i = 0; i < 24; i++)
        {
            EXPECT_EQ(time.GetElapsedDays(), 1);
            time.Update();
        }
        EXPECT_EQ(time.GetElapsedDays(), 2);
    }
    TEST_F(TimeTests, WeekdayTest)
    {
        for (auto& day : weekdays)
        {
            for (size_t i = 0; i < 24; i++)
            {
                EXPECT_EQ(time.GetCurrentDay(), day);
                time.Update();
            }
        }
    }
    TEST_F(TimeTests, WorkdayTest)
    {
        for (size_t i = 0; i < weekdays.size() - 2; i++)
        {
            for (size_t i = 0; i < 24; i++)
            {
                EXPECT_TRUE(time.IsWorkday());
                time.Update();
            }
        }
        for (size_t i = weekdays.size() - 2; i < weekdays.size(); i++)
        {
            for (size_t i = 0; i < 24; i++)
            {
                EXPECT_FALSE(time.IsWorkday());
                time.Update();
            }
        }
    }
    TEST_F(TimeTests, ClockTimeTest)
    {
        for (size_t i = 0; i < 100; i++)
        {
            for (size_t i = 0; i < 24; i++)
            {
                EXPECT_EQ(time.GetTime(), i);
                time.Update();
            }
        }
    }
}
