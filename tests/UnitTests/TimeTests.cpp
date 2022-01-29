#include "pch.h"

namespace UnitTests {
    TEST(TimeTests, SimulationTimeProgress)
    {
        auto& time = DeseaseSpreadSimulation::TimeManager::Instance();
        time.Start();

        time.Update();
        auto elapsedHours = time.GetElapsedHours();

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(49ms);
        time.Update();
        EXPECT_EQ(time.GetElapsedHours(), elapsedHours + 1ULL);

        time.Update();
        elapsedHours = time.GetElapsedHours();
        std::this_thread::sleep_for(49ms);
        time.Update();
        // Greater or equal because of small timeframe and easy errors
        EXPECT_EQ(time.GetElapsedHours(), elapsedHours + 1ULL);
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
