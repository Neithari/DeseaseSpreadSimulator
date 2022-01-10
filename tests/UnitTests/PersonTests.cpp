#include "pch.h"
#include "Person/PersonBehavior.h"

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
        std::unique_ptr<DeseaseSpreadSimulation::Home> home = std::make_unique<DeseaseSpreadSimulation::Home>();
        DeseaseSpreadSimulation::Community community;
        DeseaseSpreadSimulation::PersonBehavior behavior;
        DeseaseSpreadSimulation::Desease desease{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
    };
    TEST_F(PersonTest, ContaminateAPerson)
    {
        DeseaseSpreadSimulation::Person patient(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, community, home.get());
        patient.Contaminate(&desease);

        ASSERT_EQ(patient.GetDeseaseName(), desease.GetDeseaseName());
    }
    TEST_F(PersonTest, PersonIsInfectiousAfterLatentPeriod)
    {
        DeseaseSpreadSimulation::Person patient(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, community, home.get());
        patient.Contaminate(&desease);

        // Patient is not contagious right after contamination
        ASSERT_EQ(patient.isInfectious(), false);
        // Advance patient beyond incubation period
        patient.AdvanceDay();

        patient.Update();
        // Patient is contagious after incubation period
        ASSERT_EQ(patient.isInfectious(), true);
    }
    TEST_F(PersonTest, ContactWithOtherPersonWillInfect)
    {
        // Create 3 patients
        DeseaseSpreadSimulation::Person patient1(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, community, home.get());
        DeseaseSpreadSimulation::Person patient2(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, community, home.get());
        DeseaseSpreadSimulation::Person patient3(DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, behavior, community, home.get());
        // Contaminate 1
        patient1.Contaminate(&desease);
        // Advance patient beyond latent period
        patient1.AdvanceDay();

        patient1.Update();

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
        std::shared_ptr<DeseaseSpreadSimulation::Home> home = std::make_shared<DeseaseSpreadSimulation::Home>();
        DeseaseSpreadSimulation::Community community;

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
        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population1;

        // Setup population
        auto person = populator1.GetNewPerson(community, home.get());
        while (person)
        {
            population1.push_back(std::move(person));
            person = populator1.GetNewPerson(community, home.get());
        }

        ASSERT_EQ(population1.size(), evenCount);
    }
    TEST_F(PersonPopulatorTest, SizeIsEqualEvenDistributionUnevenCount)
    {
        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population2;

        // Setup population
        auto person = populator2.GetNewPerson(community, home.get());
        while (person)
        {
            population2.push_back(std::move(person));
            person = populator2.GetNewPerson(community, home.get());
        }

        ASSERT_EQ(population2.size(), unevenCount);
    }
    TEST_F(PersonPopulatorTest, SizeIsEqualUnevenDistributionEvenCount)
    {
        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population3;

        // Setup population
        auto person = populator3.GetNewPerson(community, home.get());
        while (person)
        {
            population3.push_back(std::move(person));
            person = populator3.GetNewPerson(community, home.get());
        }

        ASSERT_EQ(population3.size(), evenCount);
    }
    TEST_F(PersonPopulatorTest, SizeIsEqualUnevenDistributionUnevenCount)
    {
        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population4;

        // Setup population
        auto person = populator4.GetNewPerson(community, home.get());
        while (person)
        {
            population4.push_back(std::move(person));
            person = populator4.GetNewPerson(community, home.get());
        }

        ASSERT_EQ(population4.size(), unevenCount);
    }
    TEST_F(PersonPopulatorTest, EvenDistributionEvenCount)
    {
        float countHumanDistribution1 = 0.f;
        float countHumanDistribution2 = 0.f;
        float countHumanDistribution3 = 0.f;
        float countHumanDistribution4 = 0.f;

        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population1;

        // Setup population
        auto person = populator1.GetNewPerson(community, home.get());
        while (person)
        {
            population1.push_back(std::move(person));
            person = populator1.GetNewPerson(community, home.get());
        }

        for (const auto& person : population1)
        {
            DeseaseSpreadSimulation::Statistics::HumanDistribution h{ person->GetAgeGroup(), person->GetSex(), 0.f };

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

        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population2;

        // Setup population
        auto person = populator2.GetNewPerson(community, home.get());
        while (person)
        {
            population2.push_back(std::move(person));
            person = populator2.GetNewPerson(community, home.get());
        }

        for (const auto& person : population2)
        {
            DeseaseSpreadSimulation::Statistics::HumanDistribution h{ person->GetAgeGroup(), person->GetSex(), 0.f };

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

        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population3;

        // Setup population
        auto person = populator3.GetNewPerson(community, home.get());
        while (person)
        {
            population3.push_back(std::move(person));
            person = populator3.GetNewPerson(community, home.get());
        }

        for (const auto& person : population3)
        {
            DeseaseSpreadSimulation::Statistics::HumanDistribution h{ person->GetAgeGroup(), person->GetSex(), 0.f };

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

        std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>> population4;

        // Setup population
        auto person = populator4.GetNewPerson(community, home.get());
        while (person)
        {
            population4.push_back(std::move(person));
            person = populator4.GetNewPerson(community, home.get());
        }

        for (const auto& person : population4)
        {
            DeseaseSpreadSimulation::Statistics::HumanDistribution h{ person->GetAgeGroup(), person->GetSex(), 0.f };

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
}
