#include "pch.h"

namespace UnitTests {
    class CommunityTest : public ::testing::Test
    {
    protected:
        DiseaseSpreadSimulation::Community community{ std::vector<DiseaseSpreadSimulation::Person>{}, DiseaseSpreadSimulation::Places{} };
        DiseaseSpreadSimulation::PersonBehavior behavior;
    };
    TEST_F(CommunityTest, AddPlaceGetPlaces)
    {
        DiseaseSpreadSimulation::Home home1;
        uint32_t home1ID = home1.GetID();

        EXPECT_EQ(community.GetPlaces().homes.size(), 0);

        community.AddPlace(home1);
        EXPECT_EQ(community.GetPlaces().homes.size(), 1);
        ASSERT_EQ(community.GetPlaces().homes.back().GetID(), home1ID);

        DiseaseSpreadSimulation::Home home2;
        uint32_t home2ID = home2.GetID();

        community.AddPlace(home2);
        EXPECT_EQ(community.GetPlaces().homes.size(), 2);
        ASSERT_EQ(community.GetPlaces().homes.back().GetID(), home2ID);
    }
    TEST_F(CommunityTest, AddPersonGetPopulation)
    {
        DiseaseSpreadSimulation::Person person1{ DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person1ID = person1.GetID();

        EXPECT_EQ(community.GetPopulation().size(), 0);

        community.AddPerson(person1);
        EXPECT_EQ(community.GetPopulation().size(), 1);
        ASSERT_EQ(community.GetPopulation().back().GetID(), person1ID);

        DiseaseSpreadSimulation::Person person2{ DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Male, behavior, &community };
        auto person2ID = person2.GetID();
        community.AddPerson(person2);
        EXPECT_EQ(community.GetPopulation().size(), 2);
        ASSERT_EQ(community.GetPopulation().back().GetID(), person2ID);

        DiseaseSpreadSimulation::Person person3{ DiseaseSpreadSimulation::Age_Group::UnderFourty, DiseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person3ID = person3.GetID();
        community.AddPerson(person3);
        EXPECT_EQ(community.GetPopulation().size(), 3);
        ASSERT_EQ(community.GetPopulation().back().GetID(), person3ID);

        DiseaseSpreadSimulation::Person person4{ DiseaseSpreadSimulation::Age_Group::UnderFourty, DiseaseSpreadSimulation::Sex::Male, behavior, &community };
        auto person4ID = person4.GetID();
        community.AddPerson(person4);
        EXPECT_EQ(community.GetPopulation().size(), 4);
        ASSERT_EQ(community.GetPopulation().back().GetID(), person4ID);
    }
    TEST_F(CommunityTest, RemovePerson)
    {
        DiseaseSpreadSimulation::Person person1{ DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person1ID = person1.GetID();

        ASSERT_EQ(community.GetPopulation().size(), 0);

        community.AddPerson(person1);
        EXPECT_EQ(community.GetPopulation().size(), 1);
        community.RemovePerson(person1);
        ASSERT_EQ(community.GetPopulation().size(), 0);

        DiseaseSpreadSimulation::Person person2{ DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Female, behavior, &community };
        DiseaseSpreadSimulation::Person person3{ DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Female, behavior, &community };

        auto person2ID = person2.GetID();
        auto person3ID = person3.GetID();

        community.AddPerson(person2);
        community.AddPerson(person3);
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person2);
        EXPECT_EQ(community.GetPopulation().size(), 1);
        ASSERT_EQ(community.GetPopulation().back().GetID(), person3ID);
        community.RemovePerson(person3);
        ASSERT_EQ(community.GetPopulation().size(), 0);

        DiseaseSpreadSimulation::Person person4{ DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Female, behavior, &community };
        DiseaseSpreadSimulation::Person person5{ DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person4ID = person4.GetID();
        auto person5ID = person5.GetID();

        community.AddPerson(person4);
        community.AddPerson(person5);
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person1);
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person2);
        EXPECT_EQ(community.GetPopulation().size(), 2);
        community.RemovePerson(person3);
        EXPECT_EQ(community.GetPopulation().size(), 2);
    }
    TEST_F(CommunityTest, TransferPerson)
    {
        DiseaseSpreadSimulation::Person person1{ DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person1ID = person1.GetID();

        ASSERT_EQ(community.GetPopulation().size(), 0);
        community.AddPerson(person1);
        ASSERT_EQ(community.GetPopulation().size(), 1);

        auto transferPerson = community.TransferPerson(person1);
        EXPECT_EQ(community.GetPopulation().size(), 0);
        ASSERT_TRUE(transferPerson);
        ASSERT_EQ(transferPerson->GetID(), person1ID);

        DiseaseSpreadSimulation::Person person2{ DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Female, behavior, &community };
        DiseaseSpreadSimulation::Person person3{ DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Female, behavior, &community };
        DiseaseSpreadSimulation::Person person4{ DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Female, behavior, &community };
        auto person2ID = person2.GetID();
        auto person3ID = person3.GetID();
        auto person4ID = person4.GetID();

        community.AddPerson(person2);
        community.AddPerson(person3);
        community.AddPerson(person4);
        auto transferPerson2 = community.TransferPerson(person2);
        auto transferPerson3 = community.TransferPerson(person3);
        auto transferPerson4 = community.TransferPerson(person4);
        ASSERT_TRUE(transferPerson2);
        ASSERT_EQ(transferPerson2->GetID(), person2ID);
        ASSERT_TRUE(transferPerson3);
        ASSERT_EQ(transferPerson3->GetID(), person3ID);
        ASSERT_TRUE(transferPerson4);
        ASSERT_EQ(transferPerson4->GetID(), person4ID);
    }
    TEST_F(CommunityTest, TransferToPlace)
    {
        using namespace DiseaseSpreadSimulation;
        community.AddPlace(Home{});
        community.AddPlace(Supply{});
        community.AddPlace(Workplace{});
        community.AddPlace(School{});
        community.AddPlace(HardwareStore{});
        community.AddPlace(Morgue{});

        Person person{ Age_Group::UnderThirty, Sex::Female, behavior, &community, &community.GetHomes().back()};
        person.SetSchool(&community.GetPlaces().schools.back());
        person.SetWorkplace(&community.GetPlaces().workplaces.back());

        auto supply = community.TransferToSupplyStore(&person);
        EXPECT_EQ(supply->GetType(), Place_Type::Supply);

        auto home = community.TransferToHome(&person);
        EXPECT_EQ(home->GetType(), Place_Type::Home);
        EXPECT_EQ(home->GetID(), person.GetHome()->GetID());

        auto hardware = community.TransferToHardwareStore(&person);
        EXPECT_EQ(hardware->GetType(), Place_Type::HardwareStore);

        auto work = community.TransferToWork(&person);
        EXPECT_EQ(work->GetType(), Place_Type::Workplace);
        EXPECT_EQ(work->GetID(), person.GetWorkplace()->GetID());

        auto school = community.TransferToSchool(&person);
        EXPECT_EQ(school->GetType(), Place_Type::School);
        EXPECT_EQ(school->GetID(), person.GetSchool()->GetID());

        auto morgue = community.TransferToMorgue(&person);
        EXPECT_EQ(morgue->GetType(), Place_Type::Morgue);

        auto travel = community.TransferToTravelLocation(&person);
        EXPECT_EQ(travel->GetType(), Place_Type::Travel);
    }
    TEST_F(CommunityTest, AddHome)
    {
        ASSERT_TRUE(community.GetHomes().empty());

        community.AddPlace(DiseaseSpreadSimulation::Home{});
        ASSERT_FALSE(community.GetHomes().empty());
    }
    TEST_F(CommunityTest, AddSupplyStore)
    {
        ASSERT_TRUE(community.GetPlaces().supplyStores.empty());

        community.AddPlace(DiseaseSpreadSimulation::Supply{});
        ASSERT_FALSE(community.GetPlaces().supplyStores.empty());
    }
    TEST_F(CommunityTest, AddWorkplace)
    {
        ASSERT_TRUE(community.GetPlaces().workplaces.empty());

        community.AddPlace(DiseaseSpreadSimulation::Workplace{});
        ASSERT_FALSE(community.GetPlaces().workplaces.empty());
    }
    TEST_F(CommunityTest, AddSchool)
    {
        ASSERT_TRUE(community.GetPlaces().schools.empty());

        community.AddPlace(DiseaseSpreadSimulation::School{});
        ASSERT_FALSE(community.GetPlaces().schools.empty());
    }
    TEST_F(CommunityTest, AddHardwareStore)
    {
        ASSERT_TRUE(community.GetPlaces().hardwareStores.empty());

        community.AddPlace(DiseaseSpreadSimulation::HardwareStore{});
        ASSERT_FALSE(community.GetPlaces().hardwareStores.empty());
    }
    TEST_F(CommunityTest, AddMorgue)
    {
        ASSERT_TRUE(community.GetPlaces().morgues.empty());

        community.AddPlace(DiseaseSpreadSimulation::Morgue{});
        ASSERT_FALSE(community.GetPlaces().morgues.empty());
    }
}
