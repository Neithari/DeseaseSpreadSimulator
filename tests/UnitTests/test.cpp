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
    TEST(PlacesTests, GetType)
    {
        DeseaseSpreadSimulation::Home home;
        DeseaseSpreadSimulation::Supply market;
        DeseaseSpreadSimulation::Workplace work;
        DeseaseSpreadSimulation::HardwareStore hardware;
        DeseaseSpreadSimulation::Morgue morgue;

        // No test for the ID because we can't guarantee the order of the tests and ID is already checked
        EXPECT_EQ(home.GetType(), DeseaseSpreadSimulation::Place_Type::Home);
        EXPECT_EQ(market.GetType(), DeseaseSpreadSimulation::Place_Type::Supply);
        EXPECT_EQ(work.GetType(), DeseaseSpreadSimulation::Place_Type::Workplace);
        EXPECT_EQ(hardware.GetType(), DeseaseSpreadSimulation::Place_Type::HardwareStore);
        EXPECT_EQ(morgue.GetType(), DeseaseSpreadSimulation::Place_Type::Morgue);
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
}
