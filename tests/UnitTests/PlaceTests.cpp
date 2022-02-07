#include "pch.h"

namespace UnitTests {
    class PlaceTests : public ::testing::Test
    {
    protected:
        DeseaseSpreadSimulation::Home home;
        DeseaseSpreadSimulation::Community community{ std::vector<DeseaseSpreadSimulation::Person>{}, DeseaseSpreadSimulation::Places{} };
        DeseaseSpreadSimulation::PersonBehavior behavior;
    };
    TEST_F(PlaceTests, GetType)
    {
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
    TEST_F(PlaceTests, AddPerson)
    {
        DeseaseSpreadSimulation::Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, &community);
        auto personID = person.GetID();
        ASSERT_EQ(home.GetPersonCount(), 0);
        home.AddPerson(&person);
        ASSERT_EQ(home.GetPersonCount(), 1);
        ASSERT_EQ(home.GetPeople().back()->GetID(), personID);
    }
    TEST_F(PlaceTests, GetPersonCount)
    {
        DeseaseSpreadSimulation::Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, &community);
        DeseaseSpreadSimulation::Person person1(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Female, behavior, &community);
        DeseaseSpreadSimulation::Person person2(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Female, behavior, &community);

        EXPECT_EQ(home.GetPersonCount(), 0);
        home.AddPerson(&person);
        EXPECT_EQ(home.GetPersonCount(), 1);
        home.AddPerson(&person1);
        EXPECT_EQ(home.GetPersonCount(), 2);
        home.AddPerson(&person2);
        ASSERT_EQ(home.GetPersonCount(), 3);
    }
    TEST_F(PlaceTests, RemovePerson)
    {
        DeseaseSpreadSimulation::Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, &community);
        DeseaseSpreadSimulation::Person person1(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, &community);
        DeseaseSpreadSimulation::Person person2(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Female, behavior, &community);
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