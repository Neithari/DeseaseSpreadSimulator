#include "pch.h"

// Includes to be tested
#include "Desease/Desease.h"
#include "Desease/DeseaseBuilder.h"
#include "Person/Person.h"
#include "Simulation/TimeManager.h"
#include "Person/PersonPopulator.h"
#include "IDGenerator/IDGenerator.h"
#include "Places/Places.h"
#include "Places/Community.h"

namespace UnitTests {
    class DeseaseTest : public ::testing::Test
    {
    protected:
        std::string name = "a";
        int incubationPeriod = 1;
        int daysInfectious = 1;
        std::pair<int, int> deseaseDurationRange{ 2, 10 };
        std::vector<float> mortalityByAge{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        std::pair<int, int> daysTillDeathRange{ 1, 2 };
        DeseaseSpreadSimulation::Desease desease{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
  
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
        EXPECT_EQ(desease.GetID(), 0);
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
    TEST_F(DeseaseTest, EqualsOperator)
    {
        ASSERT_TRUE(desease == desease);

        DeseaseSpreadSimulation::Desease desease1{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        ASSERT_FALSE(desease == desease1);
    }
    TEST_F(DeseaseTest, IsSame)
    {
        ASSERT_TRUE(desease.isSame(desease));
        
        std::string name1 = "b";
        DeseaseSpreadSimulation::Desease desease1{ name1, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease1.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease1));
        
        int incubationPeriod1 = 2;
        DeseaseSpreadSimulation::Desease desease3{ name, incubationPeriod1, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease3.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease3));
        
        int daysInfectious1 = 2;
        DeseaseSpreadSimulation::Desease desease4{ name, incubationPeriod, daysInfectious1, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease4.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease4));
        
        std::pair<int, int> deseaseDurationRange1{ 0, 11 };
        DeseaseSpreadSimulation::Desease desease5{ name, incubationPeriod, daysInfectious, deseaseDurationRange1, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease5.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease5));
    
        std::vector<float> mortalityByAge1{ 0.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        DeseaseSpreadSimulation::Desease desease6{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge1, daysTillDeathRange };
        EXPECT_FALSE(desease6.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease6));
    
        std::pair<int, int> daysTillDeathRange1{ 0, 3 };
        DeseaseSpreadSimulation::Desease desease7{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange1 };
        EXPECT_FALSE(desease7.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease7));
    }

    TEST(DeseaseBuilderTests, DeseaseBuilderCreateCorona)
    {
        std::string name = "COVID-19";
        int incubationPeriod = 6;
        int daysInfectious = 8;
        std::pair<int, int> deseaseDurationRange{ 14, 42 };
        std::vector<float> mortalityByAge{ 0.f, 0.14f, 0.12f, 0.2f, 0.38f, 0.98f, 2.98f, 7.94f, 17.34f };
        std::pair<int, int> daysTillDeathRange{ 14, 56 };
        DeseaseSpreadSimulation::Desease corona{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };

        DeseaseSpreadSimulation::DeseaseBuilder builder;
        auto builtCorona = builder.CreateCorona();

        EXPECT_TRUE(builtCorona.isSame(corona));
    }

    class DeseaseBuilderTest : public ::testing::Test
    {
    protected:
        std::string name = "a";
        int incubationPeriod = 1;
        int daysInfectious = 1;
        std::pair<int, int> deseaseDurationRange{ 2, 10 };
        std::vector<float> mortalityByAge{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        std::pair<int, int> daysTillDeathRange{ 1, 2 };
        DeseaseSpreadSimulation::Desease desease{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
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

        DeseaseSpreadSimulation::Desease desease1{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };

        ASSERT_FALSE(builtDesease == desease);
        ASSERT_TRUE(builtDesease.GetID() > desease.GetID());

        builder.SetDeseaseName(name);
        builder.SetIncubationPeriod(incubationPeriod);
        builder.SetDaysInfectious(daysInfectious);
        builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
        builder.SetMortalityByAge(mortalityByAge);
        builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);

        auto builtDesease2 = builder.CreateDesease();

        ASSERT_TRUE(builtDesease2.GetID() > builtDesease.GetID());
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
        int incubationPeriod = 1;
        int daysInfectious = 1;
        std::pair<int, int> deseaseDurationRange{ 2, 10 };
        std::vector<float> mortalityByAge{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        std::pair<int, int> daysTillDeathRange{ 1, 2 };
        std::shared_ptr<DeseaseSpreadSimulation::Home> home = std::make_shared<DeseaseSpreadSimulation::Home>();
        DeseaseSpreadSimulation::Desease desease{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
    };
    TEST_F(PersonTest, ContaminateAPerson)
    {
        DeseaseSpreadSimulation::Person patient(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, home.get());
        patient.Contaminate(&desease);

        ASSERT_EQ(patient.GetDeseaseName(), desease.GetDeseaseName());
    }
    TEST_F(PersonTest, PersonIsInfectiousAfterLatentPeriod)
    {
        DeseaseSpreadSimulation::Person patient(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, home.get());
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
        // Create 3 patients
        DeseaseSpreadSimulation::Person patient1(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, home.get());
        DeseaseSpreadSimulation::Person patient2(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, home.get());
        DeseaseSpreadSimulation::Person patient3(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, home.get());
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
        std::shared_ptr<DeseaseSpreadSimulation::Home> home = std::make_shared<DeseaseSpreadSimulation::Home>();

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

        DeseaseSpreadSimulation::PersonPopulator populator1{ evenCount, evenDistribution };
        DeseaseSpreadSimulation::PersonPopulator populator2{ unevenCount, evenDistribution };
        DeseaseSpreadSimulation::PersonPopulator populator3{ evenCount, unevenDistribution };
        DeseaseSpreadSimulation::PersonPopulator populator4{ unevenCount, unevenDistribution };
    };
    TEST_F(PersonPopulatorTest, SizeIsEqualEvenDistributionEvenCount)
    {
        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population1;

        // Setup population
        auto person = populator1.GetNewPerson(home.get());
        while (person)
        {
            population1.push_back(std::move(person));
            person = populator1.GetNewPerson(home.get());
        }

        ASSERT_EQ(population1.size(), evenCount);
    }
    TEST_F(PersonPopulatorTest, SizeIsEqualEvenDistributionUnevenCount)
    {
        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population2;

        // Setup population
        auto person = populator2.GetNewPerson(home.get());
        while (person)
        {
            population2.push_back(std::move(person));
            person = populator2.GetNewPerson(home.get());
        }

        ASSERT_EQ(population2.size(), unevenCount);
    }
    TEST_F(PersonPopulatorTest, SizeIsEqualUnevenDistributionEvenCount)
    {
        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population3;

        // Setup population
        auto person = populator3.GetNewPerson(home.get());
        while (person)
        {
            population3.push_back(std::move(person));
            person = populator3.GetNewPerson(home.get());
        }

        ASSERT_EQ(population3.size(), evenCount);
    }
    TEST_F(PersonPopulatorTest, SizeIsEqualUnevenDistributionUnevenCount)
    {
    std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population4;

    // Setup population
    auto person = populator4.GetNewPerson(home.get());
    while (person)
    {
        population4.push_back(std::move(person));
        person = populator4.GetNewPerson(home.get());
    }

        ASSERT_EQ(population4.size(), unevenCount);
    }
    TEST_F(PersonPopulatorTest, EvenDistributionEvenCount)
    {
        float countHumanDistribution1 = 0.f;
        float countHumanDistribution2 = 0.f;
        float countHumanDistribution3 = 0.f;
        float countHumanDistribution4 = 0.f;

        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population1;

        // Setup population
        auto person = populator1.GetNewPerson(home.get());
        while (person)
        {
            population1.push_back(std::move(person));
            person = populator1.GetNewPerson(home.get());
        }

        for (const auto& person : population1)
        {
            DeseaseSpreadSimulation::PersonPopulator::HumanDistribution h{ person->GetAgeGroup(), person->GetSex(), 0.f };

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
    }
    TEST_F(PersonPopulatorTest, EvenDistributionUnevenCount)
    {
        float countHumanDistribution1 = 0.f;
        float countHumanDistribution2 = 0.f;
        float countHumanDistribution3 = 0.f;
        float countHumanDistribution4 = 0.f;

        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population2;

        // Setup population
        auto person = populator2.GetNewPerson(home.get());
        while (person)
        {
            population2.push_back(std::move(person));
            person = populator2.GetNewPerson(home.get());
        }

        for (const auto& person : population2)
        {
            DeseaseSpreadSimulation::PersonPopulator::HumanDistribution h{ person->GetAgeGroup(), person->GetSex(), 0.f };

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
    TEST_F(PersonPopulatorTest, UnevenDistributionEvenCount)
    {
        float countHumanDistribution1 = 0.f;
        float countHumanDistribution2 = 0.f;
        float countHumanDistribution3 = 0.f;
        float countHumanDistribution4 = 0.f;

        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population3;

        // Setup population
        auto person = populator3.GetNewPerson(home.get());
        while (person)
        {
            population3.push_back(std::move(person));
            person = populator3.GetNewPerson(home.get());
        }

        for (const auto& person : population3)
        {
            DeseaseSpreadSimulation::PersonPopulator::HumanDistribution h{ person->GetAgeGroup(), person->GetSex(), 0.f };

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
    }
    TEST_F(PersonPopulatorTest, UnevenDistributionUnevenCount)
    {
        float countHumanDistribution1 = 0.f;
        float countHumanDistribution2 = 0.f;
        float countHumanDistribution3 = 0.f;
        float countHumanDistribution4 = 0.f;

        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population4;

        // Setup population
        auto person = populator4.GetNewPerson(home.get());
        while (person)
        {
            population4.push_back(std::move(person));
            person = populator4.GetNewPerson(home.get());
        }

        for (const auto& person : population4)
        {
            DeseaseSpreadSimulation::PersonPopulator::HumanDistribution h{ person->GetAgeGroup(), person->GetSex(), 0.f };

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
            ASSERT_EQ(IDGenerator::IDGenerator<char>::GetNextID(), i);
        }
    }
    TEST(IDGeneratorTests, IndependentIDs)
    {
        for (size_t i = 0; i < 10; i++)
        {
            ASSERT_EQ(IDGenerator::IDGenerator<int>::GetNextID(), i);
        }

        ASSERT_EQ(IDGenerator::IDGenerator<int>::GetNextID(), 10);
        ASSERT_EQ(IDGenerator::IDGenerator<bool>::GetNextID(), 0);

        for (size_t i = 1; i < 10; i++)
        {
            ASSERT_EQ(IDGenerator::IDGenerator<bool>::GetNextID(), i);
        }
        
        ASSERT_EQ(IDGenerator::IDGenerator<std::string>::GetNextID(), 0);
        
        for (size_t i = 1; i < 10; i++)
        {
            ASSERT_EQ(IDGenerator::IDGenerator<std::string>::GetNextID(), i);
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
    TEST(PlacesTests, GetTypeName)
    {
        DeseaseSpreadSimulation::Home home;
        DeseaseSpreadSimulation::Supply market;
        DeseaseSpreadSimulation::Work work;
        DeseaseSpreadSimulation::HardwareStore hardware;
        DeseaseSpreadSimulation::Morgue morgue;

        // No test for the ID because we can't guarantee the order of the tests and ID is already checked
        EXPECT_EQ(home.GetTypeName().substr(0,4), "Home");
        EXPECT_EQ(market.GetTypeName().substr(0, 6), "Supply");
        EXPECT_EQ(work.GetTypeName().substr(0, 4), "Work");
        EXPECT_EQ(hardware.GetTypeName().substr(0, 13), "HardwareStore");
        EXPECT_EQ(morgue.GetTypeName().substr(0, 6), "Morgue");
    }
    TEST(PlacesTests, AddPerson)
    {
        DeseaseSpreadSimulation::Home home;
        DeseaseSpreadSimulation::Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male);
        auto personID = person.GetID();
        ASSERT_EQ(home.GetPersonCount(), 0);
        home.AddPerson(&person);
        ASSERT_EQ(home.GetPersonCount(), 1);
        ASSERT_EQ(home.GetPeople().back()->GetID(), personID);
    }
    TEST(PlacesTests, GetPersonCount)
    {
        DeseaseSpreadSimulation::Home home;
        DeseaseSpreadSimulation::Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male);
        DeseaseSpreadSimulation::Person person1(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male);
        DeseaseSpreadSimulation::Person person2(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male);

        EXPECT_EQ(home.GetPersonCount(), 0);
        home.AddPerson(&person);
        EXPECT_EQ(home.GetPersonCount(), 1);
        home.AddPerson(&person1);
        EXPECT_EQ(home.GetPersonCount(), 2);
        home.AddPerson(&person2);
        ASSERT_EQ(home.GetPersonCount(), 3);
    }
    TEST(PlacesTests, RemovePerson)
    {
        DeseaseSpreadSimulation::Home home;
        DeseaseSpreadSimulation::Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male);
        DeseaseSpreadSimulation::Person person1(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male);
        DeseaseSpreadSimulation::Person person2(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male);
        auto personID = person.GetID();
        auto personID1 = person1.GetID();
        auto personID2 = person2.GetID();

        home.AddPerson(&person);
        home.AddPerson(&person1);
        home.AddPerson(&person2);
        ASSERT_EQ(home.GetPersonCount(), 3);

        // Check that a wrong ID will not remove a valid person
        home.RemovePerson(12345);
        EXPECT_EQ(home.GetPersonCount(), 3);

        home.RemovePerson(personID);
        EXPECT_EQ(home.GetPersonCount(), 2);

        // Check that the right persons are inside home
        auto people = home.GetPeople();

        ASSERT_TRUE(*people.front() == person1 && *people.back() == person2);

        // Remove the rest
        home.RemovePerson(personID1);
        EXPECT_EQ(home.GetPersonCount(), 1);

        home.RemovePerson(personID2);
        EXPECT_EQ(home.GetPersonCount(), 0);

        // Check remove after no person is left
        home.RemovePerson(personID);
        ASSERT_EQ(home.GetPersonCount(), 0);
        home.RemovePerson(personID1);
        ASSERT_EQ(home.GetPersonCount(), 0);
        home.RemovePerson(personID2);
        ASSERT_EQ(home.GetPersonCount(), 0);
    }
    
    class CommunityTest : public ::testing::Test
    {
    protected:
        std::set<DeseaseSpreadSimulation::Person> population{ {DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Female},{DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male} };
        DeseaseSpreadSimulation::Community community{population};
    };
    TEST_F(CommunityTest, GetPopulation)
    {
        EXPECT_EQ(community.GetPopulation().size(), 2);

        for (const auto& person : population)
        {
            ASSERT_TRUE(community.GetPopulation().contains(person));
        }
    }
    TEST_F(CommunityTest, AddPlaceGetPlaces)
    {
        auto home1 = std::make_unique<DeseaseSpreadSimulation::Home>();
        uint32_t home1ID = home1->GetID();

        EXPECT_EQ(community.GetPlaces().size(), 0);

        community.AddPlace(std::move(home1));
        EXPECT_EQ(community.GetPlaces().size(), 1);
        ASSERT_EQ(community.GetPlaces().back()->GetID(), home1ID);

        auto home2 = std::make_unique<DeseaseSpreadSimulation::Home>();
        uint32_t home2ID = home2->GetID();

        community.AddPlace(std::move(home2));
        EXPECT_EQ(community.GetPlaces().size(), 2);
        ASSERT_EQ(community.GetPlaces().back()->GetID(), home2ID);
    }
    TEST_F(CommunityTest, AddPerson)
    {
        DeseaseSpreadSimulation::Person person1{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };

        EXPECT_EQ(community.GetPopulation().size(), 2);

        community.AddPerson(person1);
        EXPECT_EQ(community.GetPopulation().size(), 3);
        ASSERT_TRUE(community.GetPopulation().contains(person1));

        DeseaseSpreadSimulation::Person person2{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Male };
        community.AddPerson(person2);
        EXPECT_EQ(community.GetPopulation().size(), 4);
        ASSERT_TRUE(community.GetPopulation().contains(person2));

        DeseaseSpreadSimulation::Person person3{ DeseaseSpreadSimulation::Age_Group::UnderFourty, DeseaseSpreadSimulation::Sex::Female };
        community.AddPerson(person3);
        EXPECT_EQ(community.GetPopulation().size(), 5);
        ASSERT_TRUE(community.GetPopulation().contains(person3));

        DeseaseSpreadSimulation::Person person4{ DeseaseSpreadSimulation::Age_Group::UnderFourty, DeseaseSpreadSimulation::Sex::Male };
        community.AddPerson(person4);
        EXPECT_EQ(community.GetPopulation().size(), 6);
        ASSERT_TRUE(community.GetPopulation().contains(person4));
    }
    TEST_F(CommunityTest, RemovePlace)
    {
        auto home1 = std::make_unique<DeseaseSpreadSimulation::Home>();
        uint32_t home1ID = home1->GetID();

        ASSERT_EQ(community.GetPlaces().size(), 0);

        community.AddPlace(std::move(home1));
        EXPECT_EQ(community.GetPlaces().size(), 1);
        community.RemovePlace(home1ID);
        ASSERT_EQ(community.GetPlaces().size(), 0);

        auto home2 = std::make_unique<DeseaseSpreadSimulation::Home>();
        uint32_t home2ID = home2->GetID();
        auto home3 = std::make_unique<DeseaseSpreadSimulation::Home>();
        uint32_t home3ID = home3->GetID();

        community.AddPlace(std::move(home2));
        community.AddPlace(std::move(home3));
        EXPECT_EQ(community.GetPlaces().size(), 2);
        community.RemovePlace(home2ID);
        EXPECT_EQ(community.GetPlaces().size(), 1);
        ASSERT_EQ(community.GetPlaces().back()->GetID(), home3ID);
        community.RemovePlace(home3ID);
        ASSERT_EQ(community.GetPlaces().size(), 0);


        auto home4 = std::make_unique<DeseaseSpreadSimulation::Home>();
        uint32_t home4ID = home4->GetID();
        auto home5 = std::make_unique<DeseaseSpreadSimulation::Home>();
        uint32_t home5ID = home5->GetID();
        community.AddPlace(std::move(home4));
        community.AddPlace(std::move(home5));
        EXPECT_EQ(community.GetPlaces().size(), 2);
        community.RemovePlace(home1ID);
        EXPECT_EQ(community.GetPlaces().size(), 2);
        community.RemovePlace(home2ID);
        EXPECT_EQ(community.GetPlaces().size(), 2);
        community.RemovePlace(home3ID);
        EXPECT_EQ(community.GetPlaces().size(), 2);
    }
    /// TODO: void RemovePerson(const Person& person);
    TEST_F(CommunityTest, RemovePerson)
    {
    }
    /// TODO: Person TransferPerson(const Person& person);
    TEST_F(CommunityTest, TransferPerson)
    {
    }
}