#include "pch.h"

// Includes to be tested
#include "src/Desease/Desease.h"
#include "src/Desease/DeseaseBuilder.h"
#include "src/Person/Person.h"
#include "src/Simulation/TimeManager.h"
#include "src/Person/PersonPopulator.h"
#include "src/IDGenerator/IDGenerator.h"
#include "src/Places/Places.h"

namespace UnitTests {
    class DeseaseTest : public ::testing::Test
    {
    protected:
        std::string name = "a";
        unsigned int id = 0;
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
    TEST_F(DeseaseTest, MortalityByAgeGroup)
    {
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderTen),     mortalityByAge.at(0));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderTwenty),  mortalityByAge.at(1));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderThirty),  mortalityByAge.at(2));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderFourty),  mortalityByAge.at(3));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderFifty),   mortalityByAge.at(4));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderSixty),   mortalityByAge.at(5));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderSeventy), mortalityByAge.at(6));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderEighty),  mortalityByAge.at(7));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::AboveEighty),  mortalityByAge.at(8));
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
                // mortalityByAge vector does only have 9 members so prevent an out of bound
                if (index <= 8)
                {
                    EXPECT_FLOAT_EQ(desease.GetMortalityByAge(age), mortalityByAge.at(index));
                }
                else
                {
                    EXPECT_FLOAT_EQ(desease.GetMortalityByAge(age), mortalityByAge.back());
                }
            }
        }
        // Check age >=90
        for (int age = 90; age < 111; age++)
        {
            EXPECT_FLOAT_EQ(desease.GetMortalityByAge(age), mortalityByAge.back());
        }
    }
    TEST_F(DeseaseTest, IsSame)
    {
        ASSERT_TRUE(desease.isSame(desease));
        
        std::string name1 = "b";
        DeseaseSpreadSimulation::Desease desease1{ name1, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease1.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease1));
        
        unsigned int id1 = 1;
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
        unsigned int id = 0;
        int incubationPeriod = 6;
        int daysInfectious = 8;
        std::pair<int, int> deseaseDurationRange{ 14, 42 };
        std::vector<float> mortalityByAge{ 0.f, 0.14f, 0.12f, 0.2f, 0.38f, 0.98f, 2.98f, 7.94f, 17.34f };
        std::pair<int, int> daysTillDeathRange{ 14, 56 };
        DeseaseSpreadSimulation::Desease corona{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };

        DeseaseSpreadSimulation::DeseaseBuilder builder;
        auto builtCorona = builder.CreateCorona();

        EXPECT_TRUE(builtCorona.isSame(corona));
    }

    class DeseaseBuilderTest : public ::testing::Test
    {
    protected:
        std::string name = "a";
        unsigned int id = 1; // id is now 1 because the GetID function in DeseaseBuilder has advanced the id when we created corona inside DeseaseBuilderCreateCorona test
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

        auto builtDesease = builder.CreateDesease();

        ASSERT_TRUE(builtDesease.isSame(desease));
    }
    TEST_F(DeseaseBuilderTest, IDHasAdvanced)
    {
        DeseaseSpreadSimulation::DeseaseBuilder builder;
        builder.SetDeseaseName(name);
        builder.SetIncubationPeriod(incubationPeriod);
        builder.SetDaysInfectious(daysInfectious);
        builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
        builder.SetMortalityByAge(mortalityByAge);
        builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);

        auto builtDesease = builder.CreateDesease();

        DeseaseSpreadSimulation::Desease desease1{ name, ++id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };

        ASSERT_FALSE(builtDesease.isSame(desease));
        ASSERT_TRUE(builtDesease.isSame(desease1));
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
        std::pair<float, float> position{ 10.f, 10.f };

        DeseaseSpreadSimulation::Person patient(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, position);
        patient.Contaminate(&desease);

        ASSERT_EQ(patient.GetDeseaseName(), desease.GetDeseaseName());
    }
    TEST_F(PersonTest, PersonIsInfectiousAfterLatentPeriod)
    {
        std::pair<float, float> position{ 10.f, 10.f };

        DeseaseSpreadSimulation::Person patient(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, position);
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
        std::pair<float, float> position{ 10.f, 10.f };
        // Create 3 patients
        DeseaseSpreadSimulation::Person patient1(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, position);
        DeseaseSpreadSimulation::Person patient2(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, position);
        DeseaseSpreadSimulation::Person patient3(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, position);
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

    class PersonPopulatorTest : public ::testing::Test
    {
    protected:
        size_t evenCount = 100;
        size_t unevenCount = 111;

        DeseaseSpreadSimulation::PersonPopulator::HumanDistribution human1{ DeseaseSpreadSimulation::Age_Group::UnderTen, DeseaseSpreadSimulation::Sex::Male, 0.25f };
        DeseaseSpreadSimulation::PersonPopulator::HumanDistribution human2{ DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Female, 0.25f };
        DeseaseSpreadSimulation::PersonPopulator::HumanDistribution human3{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Male, 0.25f };
        DeseaseSpreadSimulation::PersonPopulator::HumanDistribution human4{ DeseaseSpreadSimulation::Age_Group::UnderFourty, DeseaseSpreadSimulation::Sex::Female, 0.25f };

        DeseaseSpreadSimulation::PersonPopulator::HumanDistribution human5{ DeseaseSpreadSimulation::Age_Group::UnderTen, DeseaseSpreadSimulation::Sex::Male, 0.10f };
        DeseaseSpreadSimulation::PersonPopulator::HumanDistribution human6{ DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Female, 0.20f };
        DeseaseSpreadSimulation::PersonPopulator::HumanDistribution human7{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Male, 0.30f };
        DeseaseSpreadSimulation::PersonPopulator::HumanDistribution human8{ DeseaseSpreadSimulation::Age_Group::UnderFourty, DeseaseSpreadSimulation::Sex::Female, 0.40f };

        std::vector<DeseaseSpreadSimulation::PersonPopulator::HumanDistribution> evenDistribution{ human1, human2, human3, human4 };
        std::vector<DeseaseSpreadSimulation::PersonPopulator::HumanDistribution> unevenDistribution{ human5, human6, human7, human8 };


        std::vector<DeseaseSpreadSimulation::Person> population1 = DeseaseSpreadSimulation::PersonPopulator::GetPopulation(evenCount, evenDistribution);
        std::vector<DeseaseSpreadSimulation::Person> population2 = DeseaseSpreadSimulation::PersonPopulator::GetPopulation(unevenCount, evenDistribution);
        std::vector<DeseaseSpreadSimulation::Person> population3 = DeseaseSpreadSimulation::PersonPopulator::GetPopulation(evenCount, unevenDistribution);
        std::vector<DeseaseSpreadSimulation::Person> population4 = DeseaseSpreadSimulation::PersonPopulator::GetPopulation(unevenCount, unevenDistribution);
    };
    TEST_F(PersonPopulatorTest, SizeIsEqualEvenCount)
    {
        ASSERT_EQ(population1.size(), evenCount);
        ASSERT_EQ(population3.size(), evenCount);
    }
    TEST_F(PersonPopulatorTest, SizeIsEqualUnevenCount)
    {
        ASSERT_EQ(population2.size(), unevenCount);
        ASSERT_EQ(population4.size(), unevenCount);
    }
    TEST_F(PersonPopulatorTest, EvenDistributionIsGood)
    {
        float countHumanDistribution1 = 0.f;
        float countHumanDistribution2 = 0.f;
        float countHumanDistribution3 = 0.f;
        float countHumanDistribution4 = 0.f;

        for (const auto& person : population1)
        {
            DeseaseSpreadSimulation::PersonPopulator::HumanDistribution h{ person.GetAgeGroup(), person.GetSex(), 0.f };
            
            if (h == human1)
            {
                countHumanDistribution1 += 1.f;
            }
            else if (h == human2)
            {
                countHumanDistribution2 += 1.f;
            }
            else if (h == human3)
            {
                countHumanDistribution3++;
            }
            else if (h == human4)
            {
                countHumanDistribution4 += 1.f;
            }
        }

        countHumanDistribution1 = countHumanDistribution1 / evenCount;
        countHumanDistribution2 = countHumanDistribution2 / evenCount;
        countHumanDistribution3 = countHumanDistribution3 / evenCount;
        countHumanDistribution4 = countHumanDistribution4 / evenCount;

        EXPECT_NEAR(countHumanDistribution1, human1.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution2, human2.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution3, human3.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution4, human4.percent, 0.01f);

        countHumanDistribution1 = 0.f;
        countHumanDistribution2 = 0.f;
        countHumanDistribution3 = 0.f;
        countHumanDistribution4 = 0.f;

        for (const auto& person : population2)
        {
            DeseaseSpreadSimulation::PersonPopulator::HumanDistribution h{ person.GetAgeGroup(), person.GetSex(), 0.f };

            if (h == human1)
            {
                countHumanDistribution1 += 1.f;
            }
            else if (h == human2)
            {
                countHumanDistribution2 += 1.f;
            }
            else if (h == human3)
            {
                countHumanDistribution3++;
            }
            else if (h == human4)
            {
                countHumanDistribution4 += 1.f;
            }
        }

        countHumanDistribution1 = countHumanDistribution1 / unevenCount;
        countHumanDistribution2 = countHumanDistribution2 / unevenCount;
        countHumanDistribution3 = countHumanDistribution3 / unevenCount;
        countHumanDistribution4 = countHumanDistribution4 / unevenCount;

        EXPECT_NEAR(countHumanDistribution1, human1.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution2, human2.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution3, human3.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution4, human4.percent, 0.01f);
    }
    TEST_F(PersonPopulatorTest, UnevenDistributionIsGood)
    {
        float countHumanDistribution1 = 0.f;
        float countHumanDistribution2 = 0.f;
        float countHumanDistribution3 = 0.f;
        float countHumanDistribution4 = 0.f;

        for (const auto& person : population3)
        {
            DeseaseSpreadSimulation::PersonPopulator::HumanDistribution h{ person.GetAgeGroup(), person.GetSex(), 0.f };

            if (h == human5)
            {
                countHumanDistribution1 += 1.f;
            }
            else if (h == human6)
            {
                countHumanDistribution2 += 1.f;
            }
            else if (h == human7)
            {
                countHumanDistribution3++;
            }
            else if (h == human8)
            {
                countHumanDistribution4 += 1.f;
            }
        }

        countHumanDistribution1 = countHumanDistribution1 / evenCount;
        countHumanDistribution2 = countHumanDistribution2 / evenCount;
        countHumanDistribution3 = countHumanDistribution3 / evenCount;
        countHumanDistribution4 = countHumanDistribution4 / evenCount;

        EXPECT_NEAR(countHumanDistribution1, human5.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution2, human6.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution3, human7.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution4, human8.percent, 0.01f);

        countHumanDistribution1 = 0.f;
        countHumanDistribution2 = 0.f;
        countHumanDistribution3 = 0.f;
        countHumanDistribution4 = 0.f;

        for (const auto& person : population4)
        {
            DeseaseSpreadSimulation::PersonPopulator::HumanDistribution h{ person.GetAgeGroup(), person.GetSex(), 0.f };

            if (h == human5)
            {
                countHumanDistribution1 += 1.f;
            }
            else if (h == human6)
            {
                countHumanDistribution2 += 1.f;
            }
            else if (h == human7)
            {
                countHumanDistribution3++;
            }
            else if (h == human8)
            {
                countHumanDistribution4 += 1.f;
            }
        }

        countHumanDistribution1 = countHumanDistribution1 / unevenCount;
        countHumanDistribution2 = countHumanDistribution2 / unevenCount;
        countHumanDistribution3 = countHumanDistribution3 / unevenCount;
        countHumanDistribution4 = countHumanDistribution4 / unevenCount;

        EXPECT_NEAR(countHumanDistribution1, human5.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution2, human6.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution3, human7.percent, 0.01f);
        EXPECT_NEAR(countHumanDistribution4, human8.percent, 0.01f);
    }

    TEST(IDGeneratorTests, IDIncrementing)
    {
        for (size_t i = 0; i < 10; i++)
        {
            EXPECT_EQ(IDGenerator::IDGenerator<char>::GetNextID(), i);
        }
    }
    TEST(IDGeneratorTests, IndependentIDs)
    {
        for (size_t i = 0; i < 10; i++)
        {
            EXPECT_EQ(IDGenerator::IDGenerator<int>::GetNextID(), i);
        }

        EXPECT_EQ(IDGenerator::IDGenerator<int>::GetNextID(), 10);
        EXPECT_EQ(IDGenerator::IDGenerator<bool>::GetNextID(), 0);

        for (size_t i = 1; i < 10; i++)
        {
            EXPECT_EQ(IDGenerator::IDGenerator<bool>::GetNextID(), i);
        }
        
        EXPECT_EQ(IDGenerator::IDGenerator<std::string>::GetNextID(), 0);
        
        for (size_t i = 1; i < 10; i++)
        {
            EXPECT_EQ(IDGenerator::IDGenerator<std::string>::GetNextID(), i);
        }
    }
    TEST(IDGeneratorTests, UserDefinedClasses)
    {
        struct Foo
        {
            int f;
        };
        struct Bar
        {
            int b;
        };

        for (size_t i = 0; i < 10; i++)
        {
            EXPECT_EQ(IDGenerator::IDGenerator<Foo>::GetNextID(), i);
            EXPECT_EQ(IDGenerator::IDGenerator<Bar>::GetNextID(), i);
        }
    }
    TEST(IDGeneratorTests, DerivedClasses)
    {
        class Base
        {
        protected:
            int b;
        };
        class Derived : public Base
        {
        private:
            int d;
        };
        for (size_t i = 0; i < 10; i++)
        {
            EXPECT_EQ(IDGenerator::IDGenerator<Base>::GetNextID(), i);
            EXPECT_EQ(IDGenerator::IDGenerator<Derived>::GetNextID(), i);
        }
    }

    TEST(PlacesTests, IDAdvancing)
    {
        DeseaseSpreadSimulation::Home home;
        DeseaseSpreadSimulation::Supply market;
        DeseaseSpreadSimulation::Work work;
        DeseaseSpreadSimulation::HardwareStore hardware;
            
        EXPECT_EQ(home.GetID(), 0);
        EXPECT_EQ(market.GetID(), 0);
        EXPECT_EQ(work.GetID(), 0);
        EXPECT_EQ(hardware.GetID(), 0);

        for (size_t i = 1; i < 10; i++)
        {
            DeseaseSpreadSimulation::Home home1;
            DeseaseSpreadSimulation::Supply market1;
            DeseaseSpreadSimulation::Work work1;
            DeseaseSpreadSimulation::HardwareStore hardware1;

            EXPECT_EQ(home1.GetID(), i);
            EXPECT_EQ(market1.GetID(), i);
            EXPECT_EQ(work1.GetID(), i);
            EXPECT_EQ(hardware1.GetID(), i);
        }
    }
    TEST(PlacesTests, GetTypeName)
    {
        DeseaseSpreadSimulation::Home home;
        DeseaseSpreadSimulation::Supply market;
        DeseaseSpreadSimulation::Work work;
        DeseaseSpreadSimulation::HardwareStore hardware;

        EXPECT_EQ(home.GetTypeName(), "Home10");
        EXPECT_EQ(market.GetTypeName(), "Supply10");
        EXPECT_EQ(work.GetTypeName(), "Work10");
        EXPECT_EQ(hardware.GetTypeName(), "HardwareStore10");
    }
}