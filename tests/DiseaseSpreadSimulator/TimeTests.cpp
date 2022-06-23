#include <gtest/gtest.h>
#include <vector>
#include "Enums.h"
#include "Simulation/TimeManager.h"

namespace UnitTests
{
	class TimeTests : public ::testing::Test
	{
	protected:
		static constexpr size_t hoursPerDay{24};
		DiseaseSpreadSimulation::TimeManager time;
		std::vector<DiseaseSpreadSimulation::Day> weekdays{
			DiseaseSpreadSimulation::Day::Monday,
			DiseaseSpreadSimulation::Day::Tuesday,
			DiseaseSpreadSimulation::Day::Wednesday,
			DiseaseSpreadSimulation::Day::Thursday,
			DiseaseSpreadSimulation::Day::Friday,
			DiseaseSpreadSimulation::Day::Saturday,
			DiseaseSpreadSimulation::Day::Sunday};
	};
	TEST_F(TimeTests, HourProgressTest)
	{
		constexpr auto testSize{100U};
		for (auto i = 0U; i < testSize; i++)
		{
			EXPECT_EQ(time.GetElapsedHours(), i);
			time.Update();
		}
	}
	// No point in splitting the test
	// NOLINTNEXTLINE(*-cognitive-complexity)
	TEST_F(TimeTests, DayProgressTest)
	{
		EXPECT_EQ(time.GetElapsedDays(), 0);

		for (size_t hour = 0; hour < hoursPerDay; hour++)
		{
			EXPECT_EQ(time.GetElapsedDays(), 0);
			time.Update();
		}
		EXPECT_EQ(time.GetElapsedDays(), 1);

		for (size_t hour = 0; hour < hoursPerDay; hour++)
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
			for (size_t hour = 0; hour < hoursPerDay; hour++)
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
			for (size_t hour = 0; hour < hoursPerDay; hour++)
			{
				EXPECT_TRUE(time.IsWorkday());
				time.Update();
			}
		}
		for (size_t i = weekdays.size() - 2; i < weekdays.size(); i++)
		{
			for (size_t hour = 0; hour < hoursPerDay; hour++)
			{
				EXPECT_FALSE(time.IsWorkday());
				time.Update();
			}
		}
	}
	TEST_F(TimeTests, ClockTimeTest)
	{
		constexpr auto testSize{100U};
		for (auto i = 0U; i < testSize; i++)
		{
			for (size_t hour = 0; hour < hoursPerDay; hour++)
			{
				EXPECT_EQ(time.GetTime(), hour);
				time.Update();
			}
		}
	}
} // namespace UnitTests
