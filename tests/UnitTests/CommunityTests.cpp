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

    TEST(CommunityBuilderTest, CreateCommunity)
    {
        DeseaseSpreadSimulation::CommunityBuilder cbuilder;

        size_t populationSize1 = 100;
        auto c1 = cbuilder.CreateCommunity(populationSize1, DeseaseSpreadSimulation::Country::USA);
        ASSERT_EQ(c1.GetPopulation().size(), populationSize1);

        size_t populationSize2 = 1000;
        auto c2 = cbuilder.CreateCommunity(populationSize2, DeseaseSpreadSimulation::Country::USA);
        ASSERT_EQ(c2.GetPopulation().size(), populationSize2);

        size_t populationSize3 = 11;
        auto c3 = cbuilder.CreateCommunity(populationSize3, DeseaseSpreadSimulation::Country::USA);
        ASSERT_EQ(c3.GetPopulation().size(), populationSize3);

        size_t populationSize4 = 673;
        auto c4 = cbuilder.CreateCommunity(populationSize4, DeseaseSpreadSimulation::Country::USA);
        ASSERT_EQ(c4.GetPopulation().size(), populationSize4);
    }
    // Helper function to get the percentages per category form a population so it can be compared to the distribution weights
    std::array<float, 4> GetHomePercentFromPopulation(const std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>>& population, DeseaseSpreadSimulation::Country country)
    {
        std::map<uint32_t, DeseaseSpreadSimulation::Home*> homesByID;

        // Get the homes of every person in the population and put it into a map to filter multiples
        for (const auto& person : population)
        {
            auto home = person->GetHome();
            homesByID.try_emplace(home->GetID(), home);
        }

        // Put the person count in every home into a vector
        std::vector<size_t> peopleCount;
        for (const auto& [id, home] : homesByID)
        {
            peopleCount.emplace_back(home->GetPeople().size());
        }

        // Count the homes separated by category
        std::array<size_t, 4> homeCount{};
        for (const auto& count : peopleCount)
        {
            // One person homes
            if (count < 2)
            {
                homeCount.at(0)++;
            }
            // 2-3 person homes
            else if (count < 4)
            {
                homeCount.at(1)++;
            }
            // 4-5 person homes
            else if (count < 6)
            {
                homeCount.at(2)++;
            }
            // over 6 person homes
            else
            {
                homeCount.at(3)++;
            }
        }
        size_t sum = homeCount.at(0) + homeCount.at(1) + homeCount.at(2) + homeCount.at(3);

        // Get the percent of the count by dividing it by the sum of all homes and check if it is near our distribution

        std::array<float, 4> percent{};
        for (size_t i = 0; i < homeCount.size(); i++)
        {
            percent.at(i) = static_cast<float>(homeCount.at(i)) / sum;
        }

        return percent;
    }
    TEST(CommunityBuilderTest, CheckHomes)
    {
        DeseaseSpreadSimulation::CommunityBuilder cbuilder;

        constexpr auto country = DeseaseSpreadSimulation::Country::USA;
        const std::array<float, 4> distributionArray{   DeseaseSpreadSimulation::PersonPopulator::GetHouseholdDistribution(country).oneMember,
                                                        DeseaseSpreadSimulation::PersonPopulator::GetHouseholdDistribution(country).twoToThreeMembers,
                                                        DeseaseSpreadSimulation::PersonPopulator::GetHouseholdDistribution(country).fourToFiveMembers,
                                                        DeseaseSpreadSimulation::PersonPopulator::GetHouseholdDistribution(country).sixPlusMembers };
        std::shared_mutex distributionArrayMutex;

        constexpr size_t populationSize1 = 1000;
        constexpr size_t testSize = 100;
        std::vector<std::thread> threads;
        threads.reserve(testSize);
        for (size_t j = 0; j < testSize; j++)
        {
            threads.emplace_back([&]() {
                auto c1 = cbuilder.CreateCommunity(populationSize1, country);

                auto homePercent1 = GetHomePercentFromPopulation(c1.GetPopulation(), country);
                for (size_t i = 0; i < homePercent1.size(); i++)
                {
                    std::shared_lock lock(distributionArrayMutex, std::defer_lock);
                    lock.lock();
                    EXPECT_NEAR(homePercent1.at(i), distributionArray.at(i), 0.18f);
                }
                });
        }
        for (auto& thread : threads)
        {
            thread.join();
        }

        size_t populationSize2 = 10000;
        auto c2 = cbuilder.CreateCommunity(populationSize2, country);
        auto homePercent2 = GetHomePercentFromPopulation(c2.GetPopulation(), country);
        for (size_t i = 0; i < homePercent2.size(); i++)
        {
            EXPECT_NEAR(homePercent2.at(i), distributionArray.at(i), 0.13f);
        }
    }
}
