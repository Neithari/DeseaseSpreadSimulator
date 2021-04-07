#include "pch.h"

// Includes to be tested
#include "src/Desease/Desease.h"
#include "src/Desease/DeseaseBuilder.h"
#include "src/Person/Person.h"
#include "src/Simulation/TimeManager.h"

namespace UnitTests {
    class DeseaseTest : public ::testing::Test
    {
    protected:
        std::string name = "a";
        int id = 0;
        int incubationPeriod = 1;
        int daysInfectious = 1;
        std::pair<int, int> deseaseDurationRange{ 2, 10 };
        std::vector<float> mortalityByAge{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        std::pair<int, int> daysTillDeathRange{ 1, 2 };
        DeseaseSpreadSimulation::Desease desease{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
  
        // Helper function to check if the given x is within the range
        bool inRange(int x, std::pair<int, int> range)
        {
            return x >= range.first && x <= range.second;
        }
    };

    TEST_F(DeseaseTest, TrivialDeseaseReturns)
    {
        EXPECT_EQ(desease.GetDeseaseName(), name);
        EXPECT_EQ(desease.IncubationPeriod(), incubationPeriod);
        EXPECT_EQ(desease.DaysInfectious(), daysInfectious);
        EXPECT_EQ(desease.GetID(), id);
    }
    TEST_F(DeseaseTest, DeseaseDurationRange)
    {
        // Check 10 times to get an average
        int duration = 0;
        for (int i = 0; i < 10; i++)
        {
            duration = desease.GetDeseaseDuration();
            ASSERT_TRUE(inRange(duration, deseaseDurationRange)) << "Desease duration: " << duration << " is not between: " << deseaseDurationRange.first << " and " << deseaseDurationRange.second;
        }
    }
    TEST_F(DeseaseTest, DaysTillDeathRange)
    {
        // Check 10 times to get an average
        int days = 0;
        for (int i = 0; i < 10; i++)
        {
            days = desease.DaysTillDeath();
            ASSERT_TRUE(inRange(days, daysTillDeathRange)) << "Days till death: " << days << " is not between: " << daysTillDeathRange.first << " and " << daysTillDeathRange.second;
        }
    }
    TEST_F(DeseaseTest, MortalityByAge)
    {
        // Check age 0-89
        int age = 0;
        for (int index = 0; index < mortalityByAge.size(); index++)
        {
            for (int i = 0; i < 10; i++)
            {
                // Age will be between 0 and 89
                age = index * 10 + i;
                EXPECT_FLOAT_EQ(mortalityByAge.at(index), desease.GetMortalityByAge(age));
            }
        }
        // Check age >=90
        for (int age = 90; age < 111; age++)
        {
            EXPECT_FLOAT_EQ(mortalityByAge.back(), desease.GetMortalityByAge(age));
        }
    }
    TEST_F(DeseaseTest, IsSame)
    {
        ASSERT_TRUE(desease.isSame(desease));
        
        std::string name1 = "b";
        DeseaseSpreadSimulation::Desease desease1{ name1, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease1.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease1));
        
        int id1 = 1;
        DeseaseSpreadSimulation::Desease desease2{ name, id1, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease2.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease2));
        
        int incubationPeriod1 = 2;
        DeseaseSpreadSimulation::Desease desease3{ name, id, incubationPeriod1, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease3.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease3));
        
        int daysInfectious1 = 2;
        DeseaseSpreadSimulation::Desease desease4{ name, id, incubationPeriod, daysInfectious1, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease4.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease4));
        
        std::pair<int, int> deseaseDurationRange1{ 0, 11 };
        DeseaseSpreadSimulation::Desease desease5{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange1, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease5.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease5));
    
        std::vector<float> mortalityByAge1{ 0.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        DeseaseSpreadSimulation::Desease desease6{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge1, daysTillDeathRange };
        EXPECT_FALSE(desease6.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease6));
    
        std::pair<int, int> daysTillDeathRange1{ 0, 3 };
        DeseaseSpreadSimulation::Desease desease7{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange1 };
        EXPECT_FALSE(desease7.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease7));
    }

    TEST(DeseaseBuilderTests, DeseaseBuilderCreateCorona)
    {
        std::string name = "COVID-19";
        int id = 0;
        int incubationPeriod = 6;
        int daysInfectious = 8;
        std::pair<int, int> deseaseDurationRange{ 14, 42 };
        std::vector<float> mortalityByAge{ 0.f, 0.14f, 0.12f, 0.2f, 0.38f, 0.98f, 2.98f, 7.94f, 17.34f };
        std::pair<int, int> daysTillDeathRange{ 14, 56 };
        DeseaseSpreadSimulation::Desease corona{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };

        DeseaseSpreadSimulation::DeseaseBuilder builder;
        auto& builtCorona = builder.CreateCorona();

        EXPECT_TRUE(builtCorona.isSame(corona));
    }
    /// TODO: implement ID set tests
    class DeseaseBuilderTest : public ::testing::Test
    {
    protected:
        std::string name = "a";
        int id = 1; // id is now 1 because the GetID function in DeseaseBuilder has advanced the id when we created corona inside DeseaseBuilderCreateCorona test
        int incubationPeriod = 1;
        int daysInfectious = 1;
        std::pair<int, int> deseaseDurationRange{ 2, 10 };
        std::vector<float> mortalityByAge{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        std::pair<int, int> daysTillDeathRange{ 1, 2 };
        DeseaseSpreadSimulation::Desease desease{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
    };
    // Tests that CreateDesease() asserts are working properly for every set function
    TEST_F(DeseaseBuilderTest, ThrowNameNotSet)
    {
        DeseaseSpreadSimulation::DeseaseBuilder builder;
        //builder.SetDeseaseName(name);
        builder.SetIncubationPeriod(incubationPeriod);
        builder.SetDaysInfectious(daysInfectious);
        builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
        builder.SetMortalityByAge(mortalityByAge);
        builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);

        ASSERT_THROW(builder.CreateDesease(), std::logic_error);
    }
    TEST_F(DeseaseBuilderTest, ThrowIncubationNotSet)
    {
        DeseaseSpreadSimulation::DeseaseBuilder builder;
        builder.SetDeseaseName(name);
        //builder.SetIncubationPeriod(incubationPeriod);
        builder.SetDaysInfectious(daysInfectious);
        builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
        builder.SetMortalityByAge(mortalityByAge);
        builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);

        ASSERT_THROW(builder.CreateDesease(), std::logic_error);
    }
    TEST_F(DeseaseBuilderTest, ThrowDaysInfectiousNotSet)
    {
        DeseaseSpreadSimulation::DeseaseBuilder builder;
        builder.SetDeseaseName(name);
        builder.SetIncubationPeriod(incubationPeriod);
        //builder.SetDaysInfectious(daysInfectious);
        builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
        builder.SetMortalityByAge(mortalityByAge);
        builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);

        ASSERT_THROW(builder.CreateDesease(), std::logic_error);
    }
    TEST_F(DeseaseBuilderTest, ThrowDeseaseDurationNotSet)
    {
        DeseaseSpreadSimulation::DeseaseBuilder builder;
        builder.SetDeseaseName(name);
        builder.SetIncubationPeriod(incubationPeriod);
        builder.SetDaysInfectious(daysInfectious);
        //builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
        builder.SetMortalityByAge(mortalityByAge);
        builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);

        ASSERT_THROW(builder.CreateDesease(), std::logic_error);
    }
    TEST_F(DeseaseBuilderTest, ThrowMortalityNotSet)
    {
        DeseaseSpreadSimulation::DeseaseBuilder builder;
        builder.SetDeseaseName(name);
        builder.SetIncubationPeriod(incubationPeriod);
        builder.SetDaysInfectious(daysInfectious);
        builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
        //builder.SetMortalityByAge(mortalityByAge);
        builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);

        ASSERT_THROW(builder.CreateDesease(), std::logic_error);
    }
    TEST_F(DeseaseBuilderTest, ThrowDaysTillDeathNotSet)
    {
        DeseaseSpreadSimulation::DeseaseBuilder builder;
        builder.SetDeseaseName(name);
        builder.SetIncubationPeriod(incubationPeriod);
        builder.SetDaysInfectious(daysInfectious);
        builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
        builder.SetMortalityByAge(mortalityByAge);
        //builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);

        ASSERT_THROW(builder.CreateDesease(), std::logic_error);
    }

    TEST_F(DeseaseBuilderTest, BuildDeseaseIsSameAsConstructed)
    {
        DeseaseSpreadSimulation::DeseaseBuilder builder;
        builder.SetDeseaseName(name);
        builder.SetIncubationPeriod(incubationPeriod);
        builder.SetDaysInfectious(daysInfectious);
        builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
        builder.SetMortalityByAge(mortalityByAge);
        builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);

        auto& builtDesease = builder.CreateDesease();

        ASSERT_TRUE(builtDesease.isSame(desease));
    }

    TEST(TimeTests, FrameTimeAfterWait)
    {
        DeseaseSpreadSimulation::TimeManager time;
        time.Update();
        using namespace std::chrono_literals;
        // Sleep for 1ms to ensure a > 1ms frametime
        std::this_thread::sleep_for(1ms);
        time.Update();
        // Check if frametime is > 1ms
        ASSERT_GE(time.GetFrameTime(), 1LL);
    }
    TEST(TimeTests, SimulationDayProgress)
    {
        DeseaseSpreadSimulation::TimeManager time;
        // Ensure that a frametime of > 50ms with a multiplier of 20 results in a simulation day
        time.SetSimulationTimeMultiplier(20);

        time.Update();
        using namespace std::chrono_literals;
        // Sleep for 50ms to ensure a > 50ms frametime
        std::this_thread::sleep_for(50ms);
        time.Update();
        EXPECT_EQ(time.GetElapsedDays(), 1ULL);
        // Sleep for 50ms to ensure a > 50ms frametime
        std::this_thread::sleep_for(50ms);
        time.Update();
        EXPECT_EQ(time.GetElapsedDays(), 2ULL);
    }

    class PersonTest : public ::testing::Test
    {
    protected:
        std::string name = "a";
        int id = 0;
        int incubationPeriod = 1;
        int daysInfectious = 1;
        std::pair<int, int> deseaseDurationRange{ 2, 10 };
        std::vector<float> mortalityByAge{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        std::pair<int, int> daysTillDeathRange{ 1, 2 };
        DeseaseSpreadSimulation::Desease desease{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
    };
    TEST_F(PersonTest, ContaminateAPerson)
    {
        int age = 10;
        std::pair<float, float> position{ 10.f, 10.f };

        DeseaseSpreadSimulation::Person patient(age, position);
        patient.Contaminate(&desease);

        ASSERT_EQ(patient.GetDeseaseName(), desease.GetDeseaseName());
    }
    TEST_F(PersonTest, PersonIsInfectiousAfterLatentPeriod)
    {
        int age = 10;
        std::pair<float, float> position{ 10.f, 10.f };

        DeseaseSpreadSimulation::Person patient(age, position);
        patient.Contaminate(&desease);

        // Patient is not contagious right after contamination
        ASSERT_EQ(patient.isInfectious(), false);
        // Advance patient beyond incubation period
        patient.AdvanceDay();

        patient.Update();
        // Patient is contagious after incubation period
        ASSERT_EQ(patient.isInfectious(), true);
    }
    TEST_F(PersonTest, ContactWithOtherPersonWillInfect)
    {
        int age = 10;
        std::pair<float, float> position{ 10.f, 10.f };
        // Create 3 patients
        DeseaseSpreadSimulation::Person patient1(age, position);
        DeseaseSpreadSimulation::Person patient2(age, position);
        DeseaseSpreadSimulation::Person patient3(age, position);
        // Contaminate 1
        patient1.Contaminate(&desease);
        // Advance patient beyond latent period
        patient1.AdvanceDay();

        patient1.Update();

        // Check non infected has contact with infected
        patient2.Contact(patient1);
        EXPECT_EQ(patient1.GetDeseaseName(), patient2.GetDeseaseName());

        // Check infected has contact with non infected
        patient1.Contact(patient3);
        EXPECT_EQ(patient1.GetDeseaseName(), patient3.GetDeseaseName());
    }
}