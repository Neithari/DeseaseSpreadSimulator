#include "pch.h"

namespace UnitTests {
    class DeseaseTest : public ::testing::Test
    {
    protected:
        std::string name = "a";
        uint16_t incubationPeriod = 1;
        uint16_t daysInfectious = 1;
        std::pair<uint16_t, uint16_t> deseaseDurationRange{ 2, 10 };
        std::vector<float> mortalityByAge{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        std::pair<uint16_t, uint16_t> daysTillDeathRange{ 1, 2 };
        DeseaseSpreadSimulation::Desease desease{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };

        // Helper function to check if the given x is within the range
        bool inRange(uint16_t x, std::pair<uint16_t, uint16_t> range)
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
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderTen), mortalityByAge.at(0));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderTwenty), mortalityByAge.at(1));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderThirty), mortalityByAge.at(2));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderFourty), mortalityByAge.at(3));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderFifty), mortalityByAge.at(4));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderSixty), mortalityByAge.at(5));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderSeventy), mortalityByAge.at(6));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::UnderEighty), mortalityByAge.at(7));
        ASSERT_EQ(desease.GetMortalityByAgeGroup(DeseaseSpreadSimulation::Age_Group::AboveEighty), mortalityByAge.at(8));
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

        uint16_t incubationPeriod1 = 2;
        DeseaseSpreadSimulation::Desease desease3{ name, incubationPeriod1, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease3.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease3));

        uint16_t daysInfectious1 = 2;
        DeseaseSpreadSimulation::Desease desease4{ name, incubationPeriod, daysInfectious1, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease4.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease4));

        std::pair<uint16_t, uint16_t> deseaseDurationRange1{ 0, 11 };
        DeseaseSpreadSimulation::Desease desease5{ name, incubationPeriod, daysInfectious, deseaseDurationRange1, mortalityByAge, daysTillDeathRange };
        EXPECT_FALSE(desease5.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease5));

        std::vector<float> mortalityByAge1{ 0.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        DeseaseSpreadSimulation::Desease desease6{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge1, daysTillDeathRange };
        EXPECT_FALSE(desease6.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease6));

        std::pair<uint16_t, uint16_t> daysTillDeathRange1{ 0, 3 };
        DeseaseSpreadSimulation::Desease desease7{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange1 };
        EXPECT_FALSE(desease7.isSame(desease));
        EXPECT_FALSE(desease.isSame(desease7));
    }

    class DeseaseBuilderTest : public ::testing::Test
    {
    protected:
        std::string name = "a";
        uint16_t incubationPeriod = 1;
        uint16_t daysInfectious = 1;
        std::pair<uint16_t, uint16_t> deseaseDurationRange{ 2, 10 };
        std::vector<float> mortalityByAge{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        std::pair<uint16_t, uint16_t> daysTillDeathRange{ 1, 2 };
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
    TEST_F(DeseaseBuilderTest, CreateCorona)
    {
        std::string name = "COVID-19";
        uint16_t incubationPeriod = 6;
        uint16_t daysInfectious = 8;
        std::pair<uint16_t, uint16_t> deseaseDurationRange{ 14, 42 };
        std::vector<float> mortalityByAge{ 0.0f, 0.0014f, 0.0012f, 0.002f, 0.0038f, 0.0098f, .0298f, .0794f, .1734f };
        std::pair<uint16_t, uint16_t> daysTillDeathRange{ 14, 56 };
        DeseaseSpreadSimulation::Desease corona{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };

        DeseaseSpreadSimulation::DeseaseBuilder builder;
        auto builtCorona = builder.CreateCorona();

        EXPECT_TRUE(builtCorona.isSame(corona));
    }
}
