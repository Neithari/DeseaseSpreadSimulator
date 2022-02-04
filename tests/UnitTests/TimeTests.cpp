#include "pch.h"

namespace UnitTests {
    TEST(TimeTests, SimulationTimeProgress)
    {
        auto& time = DeseaseSpreadSimulation::TimeManager::Instance();
        time.Start();

        for (size_t i = 0; i < 100; i++)
        {
            EXPECT_EQ(time.GetElapsedHours(), i);
            time.Update();
        }
    }
    TEST(TimeTests, TimeObserver)
    {
        auto& time = DeseaseSpreadSimulation::TimeManager::Instance();

        struct Observer : DeseaseSpreadSimulation::TimeObserver
        {
            uint16_t increasePerDay = 0;

            void OnNewDay(DeseaseSpreadSimulation::Day currentDay)
            {
                increasePerDay++;
            }
        };

        Observer observer;
        time.AddObserver(&observer);

        ASSERT_EQ(observer.increasePerDay, 0);
        for (size_t i = 1; i <= 10; i++)
        {
            time.NotifyDayChange();
            ASSERT_EQ(observer.increasePerDay, i);
        }

        auto currentIncrease = observer.increasePerDay;
        time.RemoveObserver(&observer);
        for (size_t i = 1; i <= 10; i++)
        {
            time.NotifyDayChange();
            ASSERT_EQ(observer.increasePerDay, currentIncrease);
        }
    }
}
