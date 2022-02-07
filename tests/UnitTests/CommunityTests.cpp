#include "pch.h"

namespace UnitTests {
    class CommunityTest : public ::testing::Test
    {
    protected:
        DeseaseSpreadSimulation::Community community{ std::vector<DeseaseSpreadSimulation::Person>{}, DeseaseSpreadSimulation::Places{} };
        DeseaseSpreadSimulation::PersonBehavior behavior;
    };
    TEST_F(CommunityTest, AddPlaceGetPlaces)
    {
        DeseaseSpreadSimulation::Home home1;
        uint32_t home1ID = home1.GetID();

        EXPECT_EQ(community.GetPlaces().homes.size(), 0);

        community.AddPlace(home1);
        EXPECT_EQ(community.GetPlaces().homes.size(), 1);
        ASSERT_EQ(community.GetPlaces().homes.back().GetID(), home1ID);

        DeseaseSpreadSimulation::Home home2;
        uint32_t home2ID = home2.GetID();

        community.AddPlace(home2);
        EXPECT_EQ(community.GetPlaces().homes.size(), 2);
        ASSERT_EQ(community.GetPlaces().homes.back().GetID(), home2ID);
    }
    TEST_F(CommunityTest, AddPersonGetPopulation)
    {
        DeseaseSpreadSimulation::Person person1{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person1ID = person1.GetID();

        EXPECT_EQ(community.GetPopulation().size(), 0);

        community.AddPerson(person1);
        EXPECT_EQ(community.GetPopulation().size(), 1);
        ASSERT_EQ(community.GetPopulation().back().GetID(), person1ID);

        DeseaseSpreadSimulation::Person person2{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Male, behavior, &community };
        auto person2ID = person2.GetID();
        community.AddPerson(person2);
        EXPECT_EQ(community.GetPopulation().size(), 2);
        ASSERT_EQ(community.GetPopulation().back().GetID(), person2ID);

        DeseaseSpreadSimulation::Person person3{ DeseaseSpreadSimulation::Age_Group::UnderFourty, DeseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person3ID = person3.GetID();
        community.AddPerson(person3);
        EXPECT_EQ(community.GetPopulation().size(), 3);
        ASSERT_EQ(community.GetPopulation().back().GetID(), person3ID);

        DeseaseSpreadSimulation::Person person4{ DeseaseSpreadSimulation::Age_Group::UnderFourty, DeseaseSpreadSimulation::Sex::Male, behavior, &community };
        auto person4ID = person4.GetID();
        community.AddPerson(person4);
        EXPECT_EQ(community.GetPopulation().size(), 4);
        ASSERT_EQ(community.GetPopulation().back().GetID(), person4ID);
    }
    TEST_F(CommunityTest, RemovePerson)
    {
        DeseaseSpreadSimulation::Person person1{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person1ID = person1.GetID();

        ASSERT_EQ(community.GetPopulation().size(), 0);

        community.AddPerson(person1);
        EXPECT_EQ(community.GetPopulation().size(), 1);
        community.RemovePerson(person1ID);
        ASSERT_EQ(community.GetPopulation().size(), 0);

        DeseaseSpreadSimulation::Person person2{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female, behavior, &community };
        DeseaseSpreadSimulation::Person person3{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female, behavior, &community };

        auto person2ID = person2.GetID();
        auto person3ID = person3.GetID();

        community.AddPerson(person2);
        community.AddPerson(person3);
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person2ID);
        EXPECT_EQ(community.GetPopulation().size(), 1);
        ASSERT_EQ(community.GetPopulation().back().GetID(), person3ID);
        community.RemovePerson(person3ID);
        ASSERT_EQ(community.GetPopulation().size(), 0);

        DeseaseSpreadSimulation::Person person4{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female, behavior, &community };
        DeseaseSpreadSimulation::Person person5{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person4ID = person4.GetID();
        auto person5ID = person5.GetID();

        community.AddPerson(person4);
        community.AddPerson(person5);
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person1ID);
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person2ID);
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person3ID);
        EXPECT_EQ(community.GetPopulation().size(), 2);
    }
    TEST_F(CommunityTest, TransferPerson)
    {
        DeseaseSpreadSimulation::Person person1{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person1ID = person1.GetID();

        ASSERT_EQ(community.GetPopulation().size(), 0);
        community.AddPerson(person1);
        ASSERT_EQ(community.GetPopulation().size(), 1);

        auto transferPerson = community.TransferPerson(person1ID);
        EXPECT_EQ(community.GetPopulation().size(), 0);
        ASSERT_EQ(transferPerson.GetID(), person1ID);

        DeseaseSpreadSimulation::Person person2{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female, behavior, &community };
        DeseaseSpreadSimulation::Person person3{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female, behavior, &community };
        DeseaseSpreadSimulation::Person person4{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person2ID = person2.GetID();
        auto person3ID = person3.GetID();
        auto person4ID = person4.GetID();

        community.AddPerson(person2);
        community.AddPerson(person3);
        community.AddPerson(person4);
        auto transferPerson2 = community.TransferPerson(person2ID);
        auto transferPerson3 = community.TransferPerson(person3ID);
        auto transferPerson4 = community.TransferPerson(person4ID);
        ASSERT_EQ(transferPerson2.GetID(), person2ID);
        ASSERT_EQ(transferPerson3.GetID(), person3ID);
        ASSERT_EQ(transferPerson4.GetID(), person4ID);
    }
}
