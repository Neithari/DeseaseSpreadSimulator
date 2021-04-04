// Disable/drop warnings for STL and library headers 
#ifdef _MSC_VER
#pragma warning(disable: 4290) // Suppress 'C++ Exception Specification ignored'
#pragma warning(disable: 4710) // Suppress 'function ... not inlined' for Release builds
#pragma warning(disable: 4514) // Suppress '... unreferenced inline function has been removed'
#pragma warning(disable: 4786) // Suppress '... truncated to 255 chars in debug'
#pragma warning(push, 3)       // Set warning levels to a quieter level for the STL
#endif
// Boost unit test
#define BOOST_TEST_MODULE CoronaSpreadSimulationTests
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

// Used during testing
#include <chrono>
#include <thread>

#ifdef _MSC_VER
#pragma warning(pop)			// Restore warning levels for our code
#endif

// Includes to be tested
#include "src/Desease/Desease.h"
#include "src/Desease/DeseaseBuilder.h"
#include "src/Person/Person.h"
#include "src/Simulation/TimeManager.h"

namespace UnitTests {
    struct TestDesease
    {
        std::string name = "a";
        int id = 0;
        int incubationPeriod = 1;
        int daysInfectious = 1;
        std::pair<int, int> deseaseDurationRange{ 2, 10 };
        std::vector<float> mortalityByAge{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        std::pair<int, int> daysTillDeathRange{ 1, 2 };
        DeseaseSpreadSimulation::Desease desease{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
    };

    BOOST_FIXTURE_TEST_SUITE(DeseaseTest, TestDesease)
        BOOST_AUTO_TEST_CASE(trivial_desease_returns)
        {
            BOOST_CHECK_EQUAL(desease.GetDeseaseName(), name);
            BOOST_CHECK_EQUAL(desease.IncubationPeriod(), incubationPeriod);
            BOOST_CHECK_EQUAL(desease.DaysInfectious(), daysInfectious);
        }
    
        // Helper fution to check if the given x is within the range
        bool inRange(int x, std::pair<int, int> range)
        {
            return x >= range.first && x <= range.second;
        }
    
        BOOST_AUTO_TEST_CASE(desease_duration)
        {
            // Check 10 times to get an average
            int duration = 0;
            for (int i = 0; i < 10; i++)
            {
                duration = desease.GetDeseaseDuration();
                BOOST_CHECK_MESSAGE(inRange(duration, deseaseDurationRange),
                    "Desease duration: " << duration << " is not between: " << deseaseDurationRange.first << " and " << deseaseDurationRange.second);
            }
        }
        BOOST_AUTO_TEST_CASE(days_till_death)
        {
            // Check 10 times to get an average
            int days = 0;
            for (int i = 0; i < 10; i++)
            {
                days = desease.DaysTillDeath();
                BOOST_CHECK_MESSAGE(inRange(days, daysTillDeathRange),
                    "Days till death: " << days << " is not between: " << daysTillDeathRange.first << " and " << daysTillDeathRange.second);
            }
        }
        BOOST_AUTO_TEST_CASE(mortality_by_age)
        {
            // Check age 0-89
            int age = 0;
            for (int index = 0; index < mortalityByAge.size(); index++)
            {
                for (int i = 0; i < 10; i++)
                {
                    // Age will be between 0 and 89
                    age = index * 10 + i;
                    BOOST_CHECK_CLOSE(mortalityByAge.at(index), desease.GetMortalityByAge(age), 0.001);
                }
            }
            // Check age >=90
            for (int age = 90; age < 111; age++)
            {
                BOOST_CHECK_CLOSE(mortalityByAge.back(), desease.GetMortalityByAge(age), 0.001);
            }
        }
        BOOST_FIXTURE_TEST_SUITE(BoolTests, TestDesease)
            BOOST_AUTO_TEST_CASE(is_same)
            {
                BOOST_REQUIRE(desease.isSame(desease));
    
            }
            BOOST_AUTO_TEST_CASE(is_same_test_name)
            {
                std::string name1 = "b";
                DeseaseSpreadSimulation::Desease desease2{ name1, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
                BOOST_CHECK_NE(desease2.isSame(desease), true);
                BOOST_CHECK_NE(desease.isSame(desease2), true);
            }
            BOOST_AUTO_TEST_CASE(is_same_test_id)
            {
                int id1 = 1;
                DeseaseSpreadSimulation::Desease desease2{ name, id1, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
                BOOST_CHECK_NE(desease2.isSame(desease), true);
                BOOST_CHECK_NE(desease.isSame(desease2), true);
            }
            BOOST_AUTO_TEST_CASE(is_same_test_incubation)
            {
                int incubationPeriod1 = 2;
                DeseaseSpreadSimulation::Desease desease2{ name, id, incubationPeriod1, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
                BOOST_CHECK_NE(desease2.isSame(desease), true);
                BOOST_CHECK_NE(desease.isSame(desease2), true);
            }
            BOOST_AUTO_TEST_CASE(is_same_test_daysInfectious)
            {
                int daysInfectious1 = 2;
                DeseaseSpreadSimulation::Desease desease2{ name, id, incubationPeriod, daysInfectious1, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
                BOOST_CHECK_NE(desease2.isSame(desease), true);
                BOOST_CHECK_NE(desease.isSame(desease2), true);
            }
            BOOST_AUTO_TEST_CASE(is_same_test_deseaseDuration)
            {
                std::pair<int, int> deseaseDurationRange1{ 0, 11 };
                DeseaseSpreadSimulation::Desease desease2{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange1, mortalityByAge, daysTillDeathRange };
                BOOST_CHECK_NE(desease2.isSame(desease), true);
                BOOST_CHECK_NE(desease.isSame(desease2), true);
            }
            BOOST_AUTO_TEST_CASE(is_same_test_mortality)
            {
                std::vector<float> mortalityByAge1{ 0.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
                DeseaseSpreadSimulation::Desease desease2{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge1, daysTillDeathRange };
                BOOST_CHECK_NE(desease2.isSame(desease), true);
                BOOST_CHECK_NE(desease.isSame(desease2), true);
            }
            BOOST_AUTO_TEST_CASE(is_same_test_daysTillDeath)
            {
                std::pair<int, int> daysTillDeathRange1{ 0, 3 };
                DeseaseSpreadSimulation::Desease desease2{ name, id, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange1 };
                BOOST_CHECK_NE(desease2.isSame(desease), true);
                BOOST_CHECK_NE(desease.isSame(desease2), true);
            }
        BOOST_AUTO_TEST_SUITE_END()
     BOOST_AUTO_TEST_SUITE_END()
    
        BOOST_AUTO_TEST_SUITE(DeseaseBuilderTest)
            BOOST_AUTO_TEST_CASE(build_corona)
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
    
                BOOST_CHECK(builtCorona.isSame(corona));
            }
            // Tests that CreateDesease() asserts are working propperly for every set function
            BOOST_FIXTURE_TEST_SUITE(BuildAssertTests, TestDesease)
                BOOST_AUTO_TEST_CASE(throw_name)
                {
                    DeseaseSpreadSimulation::DeseaseBuilder builder;
                    //builder.SetDeseaseName(name);
                    builder.SetIncubationPeriod(incubationPeriod);
                    builder.SetDaysInfectious(daysInfectious);
                    builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
                    builder.SetMortalityByAge(mortalityByAge);
                    builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
    
                    BOOST_REQUIRE_THROW(builder.CreateDesease(), std::logic_error);
                }
                BOOST_AUTO_TEST_CASE(throw_incubation)
                {
                    DeseaseSpreadSimulation::DeseaseBuilder builder;
                    builder.SetDeseaseName(name);
                    //builder.SetIncubationPeriod(incubationPeriod);
                    builder.SetDaysInfectious(daysInfectious);
                    builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
                    builder.SetMortalityByAge(mortalityByAge);
                    builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
    
                    BOOST_REQUIRE_THROW(builder.CreateDesease(), std::logic_error);
                }
                BOOST_AUTO_TEST_CASE(throw_daysInfectious)
                {
                    DeseaseSpreadSimulation::DeseaseBuilder builder;
                    builder.SetDeseaseName(name);
                    builder.SetIncubationPeriod(incubationPeriod);
                    //builder.SetDaysInfectious(daysInfectious);
                    builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
                    builder.SetMortalityByAge(mortalityByAge);
                    builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
    
                    BOOST_REQUIRE_THROW(builder.CreateDesease(), std::logic_error);
                }
                BOOST_AUTO_TEST_CASE(throw_deseaseDuration)
                {
                    DeseaseSpreadSimulation::DeseaseBuilder builder;
                    builder.SetDeseaseName(name);
                    builder.SetIncubationPeriod(incubationPeriod);
                    builder.SetDaysInfectious(daysInfectious);
                    //builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
                    builder.SetMortalityByAge(mortalityByAge);
                    builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
    
                    BOOST_REQUIRE_THROW(builder.CreateDesease(), std::logic_error);
                }
                BOOST_AUTO_TEST_CASE(throw_mortality)
                {
                    DeseaseSpreadSimulation::DeseaseBuilder builder;
                    builder.SetDeseaseName(name);
                    builder.SetIncubationPeriod(incubationPeriod);
                    builder.SetDaysInfectious(daysInfectious);
                    builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
                    //builder.SetMortalityByAge(mortalityByAge);
                    builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
    
                    BOOST_REQUIRE_THROW(builder.CreateDesease(), std::logic_error);
                }
                BOOST_AUTO_TEST_CASE(throw_daysTillDeath)
                {
                    DeseaseSpreadSimulation::DeseaseBuilder builder;
                    builder.SetDeseaseName(name);
                    builder.SetIncubationPeriod(incubationPeriod);
                    builder.SetDaysInfectious(daysInfectious);
                    builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
                    builder.SetMortalityByAge(mortalityByAge);
                    //builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
    
                    BOOST_REQUIRE_THROW(builder.CreateDesease(), std::logic_error);
                }
            BOOST_AUTO_TEST_SUITE_END()
            BOOST_FIXTURE_TEST_CASE(create_desease, TestDesease)
            {
                DeseaseSpreadSimulation::DeseaseBuilder builder;
                builder.SetDeseaseName(name);
                builder.SetIncubationPeriod(incubationPeriod);
                builder.SetDaysInfectious(daysInfectious);
                builder.SetDeseaseDuration(deseaseDurationRange.first, deseaseDurationRange.second);
                builder.SetMortalityByAge(mortalityByAge);
                builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
    
                auto& builtDesease = builder.CreateDesease();
    
                BOOST_CHECK(builtDesease.isSame(desease));
            }
        BOOST_AUTO_TEST_SUITE_END()
    
    
        BOOST_AUTO_TEST_SUITE(TimeTest)
            BOOST_AUTO_TEST_CASE(frameTime)
            {
                DeseaseSpreadSimulation::TimeManager time;
                time.Update();
                using namespace std::chrono_literals;
                // Sleep for 1ms to ensure a > 1ms frametime
                std::this_thread::sleep_for(1ms);
                time.Update();
                // Check if frametime is > 1ms
                BOOST_REQUIRE_GE(time.GetFrameTime(), 1LL);
            }
            BOOST_AUTO_TEST_CASE(elapsedDays)
            {
                DeseaseSpreadSimulation::TimeManager time;
                // Ensure that a frametime of > 50ms with a multiplier of 20 results in a simulation day
                time.SetSimulationTimeMultiplier(20);

                time.Update();
                using namespace std::chrono_literals;
                // Sleep for 50ms to ensure a > 50ms frametime
                std::this_thread::sleep_for(50ms);
                time.Update();
                BOOST_CHECK_EQUAL(time.GetElapsedDays(), 1ULL);
            }
        BOOST_AUTO_TEST_SUITE_END()
    
        BOOST_FIXTURE_TEST_SUITE(PersonTest, TestDesease)
            BOOST_AUTO_TEST_CASE(contaminate)
            {
                int age = 10;
                std::pair<float, float> position{ 10.f, 10.f };
    
                DeseaseSpreadSimulation::Person patient(age, position);
                patient.Contaminate(&desease);
    
                BOOST_REQUIRE_EQUAL(patient.GetDeseaseName(), desease.GetDeseaseName());
            }
            BOOST_AUTO_TEST_CASE(is_contagious)
            {
                int age = 10;
                std::pair<float, float> position{ 10.f, 10.f };
    
                DeseaseSpreadSimulation::Person patient(age, position);
                patient.Contaminate(&desease);
    
                // Patient is not contagious right after contamination
                BOOST_REQUIRE_EQUAL(patient.isInfectious(), false);
                // Advance patient beyond incubation period
                patient.AdvanceDay();
    
                patient.Update();
                // Patient is contagious after incubation period
                BOOST_REQUIRE_EQUAL(patient.isInfectious(), true);
            }
            BOOST_AUTO_TEST_CASE(contact)
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
                BOOST_CHECK_EQUAL(patient1.GetDeseaseName(), patient2.GetDeseaseName());
    
                // Check infected has contact with non infected
                patient1.Contact(patient3);
                BOOST_CHECK_EQUAL(patient1.GetDeseaseName(), patient3.GetDeseaseName());
            }
        BOOST_AUTO_TEST_SUITE_END()
}
