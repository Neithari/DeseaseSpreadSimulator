#include "pch.h"

namespace UnitTests {
    class CommunityTest : public ::testing::Test
    {
    protected:
        std::set<DeseaseSpreadSimulation::Person> population{ {DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Female},{DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male} };
        DeseaseSpreadSimulation::Community community{ population };
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
    TEST_F(CommunityTest, RemovePerson)
    {
        DeseaseSpreadSimulation::Person person1{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };

        ASSERT_EQ(community.GetPopulation().size(), 2);

        community.AddPerson(person1);
        EXPECT_EQ(community.GetPopulation().size(), 3);
        community.RemovePerson(person1);
        ASSERT_EQ(community.GetPopulation().size(), 2);

        DeseaseSpreadSimulation::Person person2{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };
        DeseaseSpreadSimulation::Person person3{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };

        community.AddPerson(person2);
        community.AddPerson(person3);
        EXPECT_EQ(community.GetPopulation().size(), 4);
        community.RemovePerson(person2);
        EXPECT_EQ(community.GetPopulation().size(), 3);
        ASSERT_TRUE(community.GetPopulation().contains(person3));
        community.RemovePerson(person3);
        ASSERT_EQ(community.GetPopulation().size(), 2);
        for (const auto& person : population)
        {
            ASSERT_TRUE(community.GetPopulation().contains(person));
        }

        DeseaseSpreadSimulation::Person person4{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };
        DeseaseSpreadSimulation::Person person5{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };
        community.AddPerson(person4);
        community.AddPerson(person5);
        EXPECT_EQ(community.GetPopulation().size(), 4);
        community.RemovePerson(person1);
        EXPECT_EQ(community.GetPopulation().size(), 4);
        community.RemovePerson(person1);
        EXPECT_EQ(community.GetPopulation().size(), 4);
        community.RemovePerson(person1);
        EXPECT_EQ(community.GetPopulation().size(), 4);
    }
    TEST_F(CommunityTest, TransferPerson)
    {
        auto transferPerson = community.TransferPerson(*population.begin());
        EXPECT_EQ(transferPerson, *population.begin());

        DeseaseSpreadSimulation::Person person1{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };
        DeseaseSpreadSimulation::Person person2{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };
        DeseaseSpreadSimulation::Person person3{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Female };

        community.AddPerson(person1);
        community.AddPerson(person2);
        community.AddPerson(person3);
        auto transferPerson1 = community.TransferPerson(person1);
        auto transferPerson2 = community.TransferPerson(person2);
        auto transferPerson3 = community.TransferPerson(person3);
        EXPECT_EQ(transferPerson1, person1);
        EXPECT_EQ(transferPerson2, person2);
        ASSERT_EQ(transferPerson3, person3);
    }
}
