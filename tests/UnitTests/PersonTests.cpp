#include "pch.h"

namespace UnitTests {
    class PersonTest : public ::testing::Test
    {
    protected:
        std::string name = "a";
        int incubationPeriod = 1;
        int daysInfectious = 1;
        std::pair<int, int> deseaseDurationRange{ 2, 10 };
        std::vector<float> mortalityByAge{ 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };
        std::pair<int, int> daysTillDeathRange{ 1, 2 };
        DeseaseSpreadSimulation::Home home;
        DeseaseSpreadSimulation::HardwareStore hwStore;
        DeseaseSpreadSimulation::Supply supplyStore;
        DeseaseSpreadSimulation::Morgue morgue;
        DeseaseSpreadSimulation::Community community{ std::vector<DeseaseSpreadSimulation::Person>{}, DeseaseSpreadSimulation::Places{} };
        DeseaseSpreadSimulation::PersonBehavior behavior{ 10u,10u,0.f };
        DeseaseSpreadSimulation::Desease desease{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
        DeseaseSpreadSimulation::TimeManager time;

        void InitCommunity()
        {
            community.AddPlace(hwStore);
            community.AddPlace(supplyStore);
            community.AddPlace(morgue);
        }
    };
    TEST_F(PersonTest, ContaminateAPerson)
    {
        DeseaseSpreadSimulation::Person patient(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, &community, &home);
        patient.Contaminate(&desease);

        ASSERT_EQ(patient.GetDeseaseName(), desease.GetDeseaseName());
    }
    TEST_F(PersonTest, PersonIsInfectiousAfterLatentPeriod)
    {
        InitCommunity();

        DeseaseSpreadSimulation::Person patient(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, &community, &home);
        patient.Contaminate(&desease);

        // Patient is not contagious right after contamination
        ASSERT_EQ(patient.IsInfectious(), false);
        // Advance patient beyond incubation period
        patient.Update(0, true, true);
        // Patient is contagious after incubation period
        ASSERT_EQ(patient.IsInfectious(), true);
    }
    TEST_F(PersonTest, ContactWithOtherPersonWillInfect)
    {
        InitCommunity();

        // Create 3 patients
        DeseaseSpreadSimulation::Person patient1(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, &community, &home);
        DeseaseSpreadSimulation::Person patient2(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, &community, &home);
        DeseaseSpreadSimulation::Person patient3(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, &community, &home);
        // Contaminate 1
        patient1.Contaminate(&desease);
        // Advance patient beyond latent period
        patient1.Update(0, true, true);

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
        DeseaseSpreadSimulation::Home home{};
        DeseaseSpreadSimulation::Community community{ std::vector<DeseaseSpreadSimulation::Person>{}, DeseaseSpreadSimulation::Places{} };

        DeseaseSpreadSimulation::Statistics::HumanDistribution human1{ DeseaseSpreadSimulation::Age_Group::UnderTen, DeseaseSpreadSimulation::Sex::Male, 0.25f };
        DeseaseSpreadSimulation::Statistics::HumanDistribution human2{ DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Female, 0.25f };
        DeseaseSpreadSimulation::Statistics::HumanDistribution human3{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Male, 0.25f };
        DeseaseSpreadSimulation::Statistics::HumanDistribution human4{ DeseaseSpreadSimulation::Age_Group::UnderFourty, DeseaseSpreadSimulation::Sex::Female, 0.25f };

        DeseaseSpreadSimulation::Statistics::HumanDistribution human5{ DeseaseSpreadSimulation::Age_Group::UnderTen, DeseaseSpreadSimulation::Sex::Male, 0.10f };
        DeseaseSpreadSimulation::Statistics::HumanDistribution human6{ DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Female, 0.20f };
        DeseaseSpreadSimulation::Statistics::HumanDistribution human7{ DeseaseSpreadSimulation::Age_Group::UnderThirty, DeseaseSpreadSimulation::Sex::Male, 0.30f };
        DeseaseSpreadSimulation::Statistics::HumanDistribution human8{ DeseaseSpreadSimulation::Age_Group::UnderFourty, DeseaseSpreadSimulation::Sex::Female, 0.40f };

        std::vector<DeseaseSpreadSimulation::Statistics::HumanDistribution> evenDistribution{ human1, human2, human3, human4 };
        std::vector<DeseaseSpreadSimulation::Statistics::HumanDistribution> unevenDistribution{ human5, human6, human7, human8 };

        DeseaseSpreadSimulation::PersonPopulator populator1{ evenCount, evenDistribution };
        DeseaseSpreadSimulation::PersonPopulator populator2{ unevenCount, evenDistribution };
        DeseaseSpreadSimulation::PersonPopulator populator3{ evenCount, unevenDistribution };
        DeseaseSpreadSimulation::PersonPopulator populator4{ unevenCount, unevenDistribution };
    };
    TEST_F(PersonPopulatorTest, SizeIsEqualEvenDistributionEvenCount)
    {
        std::vector<DeseaseSpreadSimulation::Person> population1;

        // Setup population
        auto person = populator1.GetNewPerson(&community, &home);
        population1.push_back(person);
        for (size_t i = 1; i < evenCount; i++)
        {
            person = populator1.GetNewPerson(&community, &home);
            population1.push_back(person);
        }

        ASSERT_EQ(population1.size(), evenCount);
    }
    TEST_F(PersonPopulatorTest, SizeIsEqualEvenDistributionUnevenCount)
    {
        std::vector<DeseaseSpreadSimulation::Person> population2;

        // Setup population
        auto person = populator2.GetNewPerson(&community, &home);
        population2.push_back(person);
        for (size_t i = 1; i < unevenCount; i++)
        {
            person = populator2.GetNewPerson(&community, &home);
            population2.push_back(person);
        }

        ASSERT_EQ(population2.size(), unevenCount);
    }
    TEST_F(PersonPopulatorTest, SizeIsEqualUnevenDistributionEvenCount)
    {
        std::vector<DeseaseSpreadSimulation::Person> population3;

        // Setup population
        auto person = populator3.GetNewPerson(&community, &home);
        population3.push_back(person);
        for (size_t i = 1; i < evenCount; i++)
        {
            person = populator3.GetNewPerson(&community, &home);
            population3.push_back(person);
        }

        ASSERT_EQ(population3.size(), evenCount);
    }
    TEST_F(PersonPopulatorTest, SizeIsEqualUnevenDistributionUnevenCount)
    {
        std::vector<DeseaseSpreadSimulation::Person> population4;

        // Setup population
        auto person = populator4.GetNewPerson(&community, &home);
        population4.push_back(std::move(person));
        for (size_t i = 1; i < unevenCount; i++)
        {
            person = populator4.GetNewPerson(&community, &home);
            population4.push_back(std::move(person));
        }

        ASSERT_EQ(population4.size(), unevenCount);
    }
    TEST_F(PersonPopulatorTest, EvenDistributionEvenCount)
    {
        float countHumanDistribution1 = 0.f;
        float countHumanDistribution2 = 0.f;
        float countHumanDistribution3 = 0.f;
        float countHumanDistribution4 = 0.f;

        std::vector<DeseaseSpreadSimulation::Person> population1;

        // Setup population
        auto person1 = populator1.GetNewPerson(&community, &home);
        population1.push_back(std::move(person1));
        for (size_t i = 1; i < evenCount; i++)
        {
            person1 = populator1.GetNewPerson(&community, &home);
            population1.push_back(std::move(person1));
        }

        for (const auto& person : population1)
        {
            DeseaseSpreadSimulation::Statistics::HumanDistribution h{ person.GetAgeGroup(), person.GetSex(), 0.f };

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

        std::vector<DeseaseSpreadSimulation::Person> population2;

        // Setup population
        auto person2 = populator2.GetNewPerson(&community, &home);
        population2.push_back(std::move(person2));
        for (size_t i = 1; i < unevenCount; i++)
        {
            person2 = populator2.GetNewPerson(&community, &home);
            population2.push_back(std::move(person2));
        }

        for (const auto& person : population2)
        {
            DeseaseSpreadSimulation::Statistics::HumanDistribution h{ person.GetAgeGroup(), person.GetSex(), 0.f };

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

        std::vector<DeseaseSpreadSimulation::Person> population3;

        // Setup population
        auto person3 = populator3.GetNewPerson(&community, &home);
        population3.push_back(std::move(person3));
        for (size_t i = 1; i < evenCount; i++)
        {
            person3 = populator3.GetNewPerson(&community, &home);
            population3.push_back(std::move(person3));
        }

        for (const auto& person : population3)
        {
            DeseaseSpreadSimulation::Statistics::HumanDistribution h{ person.GetAgeGroup(), person.GetSex(), 0.f };

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

        std::vector<DeseaseSpreadSimulation::Person> population4;

        // Setup population
        auto person4 = populator4.GetNewPerson(&community, &home);
        population4.push_back(std::move(person4));
        for (size_t i = 1; i < unevenCount; i++)
        {
            person4 = populator4.GetNewPerson(&community, &home);
            population4.push_back(std::move(person4));
        }

        for (const auto& person : population4)
        {
            DeseaseSpreadSimulation::Statistics::HumanDistribution h{ person.GetAgeGroup(), person.GetSex(), 0.f };

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
    // Helper function to get the percentages per category form a population so it can be compared to the distribution weights
    std::array<float, 4> GetHomePercentFromPopulation(std::vector<DeseaseSpreadSimulation::Person>& population, DeseaseSpreadSimulation::Country country)
    {
        std::map<uint32_t, DeseaseSpreadSimulation::Home*> homesByID;

        // Get the homes of every person in the population and put it into a map to filter multiples
        for (auto& person : population)
        {
            auto home = person.GetHome();
            homesByID.try_emplace(home->GetID(), home);
        }

        // Put the person count in every home into a vector
        std::vector<size_t> peopleCount;
        for (auto& [id, home] : homesByID)
        {
            peopleCount.emplace_back(home->GetPeople().size());
        }

        // Count the homes separated by category
        std::array<size_t, 4> homeCount{};
        for (auto& count : peopleCount)
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
    TEST_F(PersonPopulatorTest, CheckHomes)
    {
        using namespace DeseaseSpreadSimulation;
        constexpr auto country = Country::USA;
        const std::array<float, 4> distributionArray{ PersonPopulator::GetHouseholdDistribution(country).oneMember,
                                                        PersonPopulator::GetHouseholdDistribution(country).twoToThreeMembers,
                                                        PersonPopulator::GetHouseholdDistribution(country).fourToFiveMembers,
                                                        PersonPopulator::GetHouseholdDistribution(country).sixPlusMembers };
        std::shared_mutex distributionArrayMutex;

        constexpr size_t populationSize1 = 1000;
        constexpr size_t testSize = 20;
        std::vector<std::thread> threads;
        threads.reserve(testSize);
        for (size_t j = 0; j < testSize; j++)
        {
            threads.emplace_back([&]() {
                PlaceBuilder placeFactory;
                PersonPopulator populationFactory(populationSize1, PersonPopulator::GetCountryDistribution(country));
                auto places = placeFactory.CreatePlaces(populationSize1, country);
                auto population = populationFactory.CreatePopulation(populationSize1, country, places.homes, places.workplaces);

                auto homePercent1 = GetHomePercentFromPopulation(population, country);
                for (size_t i = 0; i < homePercent1.size(); i++)
                {
                    std::shared_lock lock(distributionArrayMutex, std::defer_lock);
                    lock.lock();
                    EXPECT_NEAR(homePercent1.at(i), distributionArray.at(i), 0.2f);
                }
                });
        }
        for (auto& thread : threads)
        {
            thread.join();
        }

        size_t populationSize2 = 10000;
        PlaceBuilder placeFactory;
        PersonPopulator populationFactory(populationSize2, PersonPopulator::GetCountryDistribution(country));
        auto places = placeFactory.CreatePlaces(populationSize2, country);
        auto population = populationFactory.CreatePopulation(populationSize1, country, places.homes, places.workplaces);

        auto homePercent2 = GetHomePercentFromPopulation(population, country);
        for (size_t i = 0; i < homePercent2.size(); i++)
        {
            EXPECT_NEAR(homePercent2.at(i), distributionArray.at(i), 0.13f);
        }
    }
}
