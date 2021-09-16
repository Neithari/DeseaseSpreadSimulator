#include "pch.h"

namespace UnitTests {
    class CommunityTest : public ::testing::Test
    {
    protected:
        DeseaseSpreadSimulation::Community community;
    };
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
    TEST_F(CommunityTest, AddPersonGetPopulation)
    {
        DeseaseSpreadSimulation::Person person1{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };

        EXPECT_EQ(community.GetPopulation().size(), 0);

        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person1));
        EXPECT_EQ(community.GetPopulation().size(), 1);
        ASSERT_EQ(community.GetPopulation().back()->GetID(), person1.GetID());

        DeseaseSpreadSimulation::Person person2{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Male };
        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person2));
        EXPECT_EQ(community.GetPopulation().size(), 2);
        ASSERT_EQ(community.GetPopulation().back()->GetID(), person2.GetID());

        DeseaseSpreadSimulation::Person person3{ DeseaseSpreadSimulation::Age_Group::UnderFourty, DeseaseSpreadSimulation::Sex::Female };
        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person3));
        EXPECT_EQ(community.GetPopulation().size(), 3);
        ASSERT_EQ(community.GetPopulation().back()->GetID(), person3.GetID());

        DeseaseSpreadSimulation::Person person4{ DeseaseSpreadSimulation::Age_Group::UnderFourty, DeseaseSpreadSimulation::Sex::Male };
        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person4));
        EXPECT_EQ(community.GetPopulation().size(), 4);
        ASSERT_EQ(community.GetPopulation().back()->GetID(), person4.GetID());
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
    TEST_F(CommunityTest, RemovePerson)
    {
        DeseaseSpreadSimulation::Person person1{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };

        ASSERT_EQ(community.GetPopulation().size(), 0);

        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person1));
        EXPECT_EQ(community.GetPopulation().size(), 1);
        community.RemovePerson(person1.GetID());
        ASSERT_EQ(community.GetPopulation().size(), 0);

        DeseaseSpreadSimulation::Person person2{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };
        DeseaseSpreadSimulation::Person person3{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };

        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person2));
        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person3));
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person2.GetID());
        EXPECT_EQ(community.GetPopulation().size(), 1);
        ASSERT_EQ(community.GetPopulation().back()->GetID(), person3.GetID());
        community.RemovePerson(person3.GetID());
        ASSERT_EQ(community.GetPopulation().size(), 0);

        DeseaseSpreadSimulation::Person person4{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };
        DeseaseSpreadSimulation::Person person5{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };
        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person4));
        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person5));
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person1.GetID());
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person2.GetID());
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person3.GetID());
        EXPECT_EQ(community.GetPopulation().size(), 2);
    }
    TEST_F(CommunityTest, TransferPerson)
    {
        DeseaseSpreadSimulation::Person person1{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };

        ASSERT_EQ(community.GetPopulation().size(), 0);
        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person1));
        ASSERT_EQ(community.GetPopulation().size(), 1);

        auto transferPerson = community.TransferPerson(person1.GetID());
        EXPECT_EQ(community.GetPopulation().size(), 0);
        ASSERT_EQ(*transferPerson, person1);

        DeseaseSpreadSimulation::Person person2{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };
        DeseaseSpreadSimulation::Person person3{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };
        DeseaseSpreadSimulation::Person person4{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };

        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person2));
        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person3));
        community.AddPerson(std::make_unique<DeseaseSpreadSimulation::Person>(person4));
        auto transferPerson2 = community.TransferPerson(person2.GetID());
        auto transferPerson3 = community.TransferPerson(person3.GetID());
        auto transferPerson4 = community.TransferPerson(person4.GetID());
        ASSERT_EQ(*transferPerson2, person2);
        ASSERT_EQ(*transferPerson3, person3);
        ASSERT_EQ(*transferPerson4, person4);
    }
}
