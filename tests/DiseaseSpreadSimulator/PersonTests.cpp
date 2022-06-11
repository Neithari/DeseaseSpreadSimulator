#include "pch.h"

namespace UnitTests
{
	class PersonTest : public ::testing::Test
	{
	protected:
		std::string name = "a";
		std::pair<uint16_t, uint16_t> incubationPeriod{1, 1};
		uint16_t daysInfectious = 1;
		std::pair<uint16_t, uint16_t> diseaseDurationRange{2, 10};
		std::vector<float> mortalityByAge{1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f};
		std::pair<uint16_t, uint16_t> daysTillDeathRange{1, 2};
		std::pair<float, float> spreadFactor{1.f, 1.f};
		float testAccuracy{1.0f};
		std::pair<float, float> symptomsDevelopment{1.f, 1.f};
		std::vector<DiseaseSpreadSimulation::Home> homes{DiseaseSpreadSimulation::Home{}};
		DiseaseSpreadSimulation::HardwareStore hwStore;
		DiseaseSpreadSimulation::Supply supplyStore;
		DiseaseSpreadSimulation::Morgue morgue;
		DiseaseSpreadSimulation::Workplace work;
		DiseaseSpreadSimulation::School school;
		DiseaseSpreadSimulation::Community community{std::vector<DiseaseSpreadSimulation::Person>{}, DiseaseSpreadSimulation::Places{}};
		DiseaseSpreadSimulation::PersonBehavior behavior{10u, 10u, 0.f, 0.f};
		DiseaseSpreadSimulation::Disease disease{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		DiseaseSpreadSimulation::TimeManager time;
		// Values from Person.h
		static constexpr uint16_t shopOpenTime = 7u;
		static constexpr uint16_t shopCloseTime = 20u;
		static constexpr uint16_t workStartTime = 8u;
		static constexpr uint16_t workFinishTime = 17u;
		static constexpr uint16_t schoolStartTime = 8u;
		static constexpr uint16_t schoolFinishTime = 15u;

		void InitCommunity()
		{

			community.AddPlace(homes.back());
			community.AddPlace(hwStore);
			community.AddPlace(supplyStore);
			community.AddPlace(morgue);
			community.AddPlace(work);
			community.AddPlace(school);
		}
	};
	TEST_F(PersonTest, UpdateSupplyMoves)
	{
		InitCommunity();

		using namespace DiseaseSpreadSimulation;
		PersonBehavior foodBuyBehavior{0, 100, 1.f, 0.f};

		Person person1(Age_Group::UnderThirty, Sex::Female, foodBuyBehavior, &community, &homes.back());
		ASSERT_EQ(person1.GetWhereabouts(), person1.GetHome());

		// Check shopping for supplies
		// We shop at a random time between open and close. Because of that we need to check the whole time range
		bool wasShopping = false;
		for (auto i = shopOpenTime; i <= shopCloseTime; i++)
		{
			person1.Update(i, true, false);
			if (person1.GetWhereabouts()->GetType() == Place_Type::Supply)
			{
				wasShopping = true;
			}
		}
		EXPECT_TRUE(wasShopping);

		// Check harware shop after supply
		Person person2(Age_Group::UnderThirty, Sex::Female, foodBuyBehavior, &community, &homes.back());
		ASSERT_EQ(person2.GetWhereabouts(), person2.GetHome());

		uint16_t buyEndTime = 0;
		for (auto i = shopOpenTime; i <= shopCloseTime; i++)
		{
			person2.Update(i, true, false);
			if (person2.GetWhereabouts()->GetType() == Place_Type::Supply)
			{
				buyEndTime = i + 1;
				break;
			}
		}
		ASSERT_EQ(person2.GetWhereabouts()->GetType(), Place_Type::Supply);
		PersonBehavior hardwareBuyBehavior{100, 0, 1.f, 0.f};
		person2.ChangeBehavior(hardwareBuyBehavior);
		person2.Update(buyEndTime, true, false);
		ASSERT_EQ(person2.GetWhereabouts()->GetType(), Place_Type::HardwareStore);

		// Check home shop after supply
		Person person3(Age_Group::UnderThirty, Sex::Female, foodBuyBehavior, &community, &homes.back());
		ASSERT_EQ(person3.GetWhereabouts(), person3.GetHome());

		buyEndTime = 0;
		for (auto i = shopOpenTime; i <= shopCloseTime; i++)
		{
			person3.Update(i, true, false);
			if (person3.GetWhereabouts()->GetType() == Place_Type::Supply)
			{
				buyEndTime = i + 1;
				break;
			}
		}
		ASSERT_EQ(person3.GetWhereabouts()->GetType(), Place_Type::Supply);
		person3.Update(buyEndTime, true, false);
		ASSERT_EQ(person3.GetWhereabouts(), person3.GetHome());
	}
	TEST_F(PersonTest, UpdateHardwareMoves)
	{
		InitCommunity();

		using namespace DiseaseSpreadSimulation;
		PersonBehavior hardwareBuyBehavior{100, 0, 1.f, 0.f};

		Person person1(Age_Group::UnderThirty, Sex::Male, hardwareBuyBehavior, &community, &homes.back());
		ASSERT_EQ(person1.GetWhereabouts(), person1.GetHome());

		// We shop at a random time between open and close. Because of that we need to check the whole time range
		bool wasShopping = false;

		for (auto i = shopOpenTime; i <= shopCloseTime; i++)
		{
			person1.Update(i, true, false);
			if (person1.GetWhereabouts()->GetType() == Place_Type::HardwareStore)
			{
				wasShopping = true;
			}
		}
		EXPECT_TRUE(wasShopping);

		// Check harware shop after supply
		Person person2(Age_Group::UnderThirty, Sex::Female, hardwareBuyBehavior, &community, &homes.back());
		ASSERT_EQ(person2.GetWhereabouts(), person2.GetHome());

		uint16_t buyEndTime = 0;
		for (auto i = shopOpenTime; i <= shopCloseTime; i++)
		{
			person2.Update(i, true, false);
			if (person2.GetWhereabouts()->GetType() == Place_Type::HardwareStore)
			{
				buyEndTime = i + 1;
				break;
			}
		}
		ASSERT_EQ(person2.GetWhereabouts()->GetType(), Place_Type::HardwareStore);
		PersonBehavior foodBuyBehavior{0, 100, 1.f, 0.f};
		person2.ChangeBehavior(foodBuyBehavior);
		person2.Update(buyEndTime, true, false);
		ASSERT_EQ(person2.GetWhereabouts()->GetType(), Place_Type::Supply);

		// Check home shop after supply
		Person person3(Age_Group::UnderThirty, Sex::Female, hardwareBuyBehavior, &community, &homes.back());
		ASSERT_EQ(person3.GetWhereabouts(), person3.GetHome());

		buyEndTime = 0;
		for (auto i = shopOpenTime; i <= shopCloseTime; i++)
		{
			person3.Update(i, true, false);
			if (person3.GetWhereabouts()->GetType() == Place_Type::HardwareStore)
			{
				buyEndTime = i + 1;
				break;
			}
		}
		ASSERT_EQ(person3.GetWhereabouts()->GetType(), Place_Type::HardwareStore);
		person3.Update(buyEndTime, true, false);
		ASSERT_EQ(person3.GetWhereabouts(), person3.GetHome());
	}
	TEST_F(PersonTest, UpdateTravelMoves)
	{
		InitCommunity();

		using namespace DiseaseSpreadSimulation;
		PersonBehavior travelBehavior{100, 100, 0.f, 1.f};

		Person traveler(Age_Group::UnderThirty, Sex::Female, travelBehavior, &community, &homes.back());
		ASSERT_EQ(traveler.GetWhereabouts(), traveler.GetHome());

		traveler.Update(shopOpenTime, false, false);
		EXPECT_EQ(traveler.GetWhereabouts()->GetType(), Place_Type::Travel);
		EXPECT_TRUE(traveler.IsTraveling());

		// Update with a new day untill we are no longer traveling
		while (traveler.IsTraveling())
		{
			traveler.Update(0, false, true);
		}
		ASSERT_EQ(traveler.GetWhereabouts(), traveler.GetHome());
	}
	TEST_F(PersonTest, UpdateWorkMoves)
	{
		InitCommunity();

		using namespace DiseaseSpreadSimulation;
		PersonBehavior workerBehavior{100, 100, 1.f, 0.f};

		Person worker(Age_Group::UnderThirty, Sex::Female, workerBehavior, &community, &homes.back());
		ASSERT_EQ(worker.GetWhereabouts(), worker.GetHome());
		worker.SetWorkplace(&work);

		for (auto i = workStartTime; i < workFinishTime; i++)
		{
			worker.Update(i, true, false);
			EXPECT_EQ(worker.GetWhereabouts(), worker.GetWorkplace());
		}
		// Check getting back home
		worker.Update(workFinishTime, true, false);
		ASSERT_EQ(worker.GetWhereabouts(), worker.GetHome());

		// Check travel after work
		worker.Update(workStartTime, true, false);
		ASSERT_EQ(worker.GetWhereabouts(), worker.GetWorkplace());
		PersonBehavior travelBehavior{100, 100, 0.f, 1.f};
		worker.ChangeBehavior(travelBehavior);

		worker.Update(workFinishTime, true, false);
		EXPECT_EQ(worker.GetWhereabouts()->GetType(), Place_Type::Travel);
	}
	TEST_F(PersonTest, UpdateSchoolMoves)
	{
		InitCommunity();

		using namespace DiseaseSpreadSimulation;
		PersonBehavior schoolBehavior{100, 100, 1.f, 0.f};

		Person schoolKid(Age_Group::UnderThirty, Sex::Male, schoolBehavior, &community, &homes.back());
		ASSERT_EQ(schoolKid.GetWhereabouts(), schoolKid.GetHome());
		schoolKid.SetSchool(&school);

		for (auto i = schoolStartTime; i < schoolFinishTime; i++)
		{
			schoolKid.Update(i, true, false);
			EXPECT_EQ(schoolKid.GetWhereabouts(), schoolKid.GetSchool());
		}
		// Check getting back home
		schoolKid.Update(schoolFinishTime, true, false);
		EXPECT_EQ(schoolKid.GetWhereabouts(), schoolKid.GetHome());
	}
	TEST_F(PersonTest, UpdateMorgueMoves)
	{
		InitCommunity();

		using namespace DiseaseSpreadSimulation;

		Person person(Age_Group::UnderThirty, Sex::Female, behavior, &community, &homes.back());
		ASSERT_EQ(person.GetWhereabouts(), person.GetHome());
		ASSERT_TRUE(person.IsAlive());

		person.Kill();
		ASSERT_FALSE(person.IsAlive());
		person.Update(0, false, false);
		ASSERT_EQ(person.GetWhereabouts()->GetType(), Place_Type::Morgue);

		// Check that the person stays in morgue
		for (size_t i = 0; i < 1000; i++)
		{
			for (uint16_t h = 0; h < 24; h++)
			{
				person.Update(h, true, true);
				ASSERT_EQ(person.GetWhereabouts()->GetType(), Place_Type::Morgue);
			}
			// Check other random behaviors
			if (i % 5 == 0)
			{
				person.ChangeBehavior(PersonBehavior{});
			}
		}
	}
	TEST_F(PersonTest, UpdateInfection)
	{
		InitCommunity();
		using namespace DiseaseSpreadSimulation;

		Person person(Age_Group::UnderTwenty, Sex::Male, behavior, &community, &homes.back());

		ASSERT_FALSE(person.HasDisease());
		person.Contaminate(&disease);
		ASSERT_TRUE(person.HasDisease());
		ASSERT_FALSE(person.IsInfectious());
		// Advance patient beyond latent period
		person.Update(0, true, true);
		ASSERT_TRUE(person.IsInfectious());
	}
	TEST_F(PersonTest, ContactWithOtherPersonWillInfect)
	{
		InitCommunity();

		// Create 3 patients
		DiseaseSpreadSimulation::Person patient1(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, &community, &homes.back());
		DiseaseSpreadSimulation::Person patient2(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, &community, &homes.back());
		DiseaseSpreadSimulation::Person patient3(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, &community, &homes.back());
		// Contaminate 1
		patient1.Contaminate(&disease);
		// Advance patient beyond latent period
		patient1.Update(0, true, true);
		ASSERT_TRUE(patient1.IsInfectious());

		// Check non infected has contact with infected
		do
		{
			patient2.Contact(patient1);
		} while (!patient2.HasDisease());
		EXPECT_EQ(patient1.GetDiseaseName(), patient2.GetDiseaseName());

		// Check infected has contact with non infected
		do
		{
			patient1.Contact(patient3);
		} while (!patient3.HasDisease());
		EXPECT_EQ(patient1.GetDiseaseName(), patient3.GetDiseaseName());
	}
	TEST_F(PersonTest, ContaminateAPerson)
	{
		InitCommunity();

		DiseaseSpreadSimulation::Person patient(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, &community, &homes.back());

		ASSERT_FALSE(patient.HasDisease());
		patient.Contaminate(&disease);
		ASSERT_TRUE(patient.HasDisease());

		ASSERT_EQ(patient.GetDiseaseName(), disease.GetDiseaseName());
	}
	TEST_F(PersonTest, PersonIsInfectiousAfterLatentPeriod)
	{
		InitCommunity();

		DiseaseSpreadSimulation::Person patient(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, &community, &homes.back());
		patient.Contaminate(&disease);

		// Patient is not contagious right after contamination
		ASSERT_EQ(patient.IsInfectious(), false);
		// Advance patient beyond incubation period
		patient.Update(0, true, true);
		// Patient is contagious after incubation period
		ASSERT_EQ(patient.IsInfectious(), true);
	}
	TEST_F(PersonTest, Kill)
	{
		InitCommunity();

		DiseaseSpreadSimulation::Person person(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, &community, &homes.back());

		ASSERT_TRUE(person.IsAlive());
		person.Kill();
		ASSERT_FALSE(person.IsAlive());
	}
	TEST_F(PersonTest, IDTest)
	{
		InitCommunity();

		DiseaseSpreadSimulation::Person person1(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, &community, &homes.back());
		DiseaseSpreadSimulation::Person person2(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, &community, &homes.back());

		ASSERT_NE(person1.GetID(), person2.GetID());
	}
	TEST_F(PersonTest, ChangeBehavior)
	{
		InitCommunity();

		using namespace DiseaseSpreadSimulation;
		// Create 3 patients
		Person person1(Age_Group::UnderTwenty, Sex::Male, behavior, &community, &homes.back());

		auto& behavior = person1.GetBehavior();
		PersonBehavior oldBehavior(behavior.foodBuyInterval, behavior.hardwareBuyInterval, behavior.acceptanceFactor, behavior.travelNeed);

		person1.ChangeBehavior(PersonBehavior{});
		auto& currentBehavior = person1.GetBehavior();
		EXPECT_NE(currentBehavior.foodBuyInterval, oldBehavior.foodBuyInterval);
		EXPECT_NE(currentBehavior.hardwareBuyInterval, oldBehavior.hardwareBuyInterval);
		EXPECT_NE(currentBehavior.acceptanceFactor, oldBehavior.acceptanceFactor);
		EXPECT_NE(currentBehavior.travelNeed, oldBehavior.travelNeed);
	}

	class PersonPopulatorTest : public ::testing::Test
	{
	protected:
		static constexpr size_t evenCount{100};
		static constexpr size_t unevenCount{111};
		DiseaseSpreadSimulation::Country country{DiseaseSpreadSimulation::Country::USA};
		DiseaseSpreadSimulation::Community community{std::vector<DiseaseSpreadSimulation::Person>{}, DiseaseSpreadSimulation::Places{}};

		DiseaseSpreadSimulation::Statistics::HumanDistribution human1{DiseaseSpreadSimulation::Age_Group::UnderTen, DiseaseSpreadSimulation::Sex::Male, 0.25f};
		DiseaseSpreadSimulation::Statistics::HumanDistribution human2{DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Female, 0.25f};
		DiseaseSpreadSimulation::Statistics::HumanDistribution human3{DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Male, 0.25f};
		DiseaseSpreadSimulation::Statistics::HumanDistribution human4{DiseaseSpreadSimulation::Age_Group::UnderFourty, DiseaseSpreadSimulation::Sex::Female, 0.25f};

		DiseaseSpreadSimulation::Statistics::HumanDistribution human5{DiseaseSpreadSimulation::Age_Group::UnderTen, DiseaseSpreadSimulation::Sex::Male, 0.10f};
		DiseaseSpreadSimulation::Statistics::HumanDistribution human6{DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Female, 0.20f};
		DiseaseSpreadSimulation::Statistics::HumanDistribution human7{DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Male, 0.30f};
		DiseaseSpreadSimulation::Statistics::HumanDistribution human8{DiseaseSpreadSimulation::Age_Group::UnderFourty, DiseaseSpreadSimulation::Sex::Female, 0.40f};

		std::vector<DiseaseSpreadSimulation::Statistics::HumanDistribution> evenDistribution{human1, human2, human3, human4};
		std::vector<DiseaseSpreadSimulation::Statistics::HumanDistribution> unevenDistribution{human5, human6, human7, human8};

		DiseaseSpreadSimulation::PersonPopulator populator1{evenCount, evenDistribution};
		DiseaseSpreadSimulation::PersonPopulator populator2{unevenCount, evenDistribution};
		DiseaseSpreadSimulation::PersonPopulator populator3{evenCount, unevenDistribution};
		DiseaseSpreadSimulation::PersonPopulator populator4{unevenCount, unevenDistribution};
	};
	TEST_F(PersonPopulatorTest, SizeIsEqualEvenDistributionEvenCount)
	{
		// Setup population
		DiseaseSpreadSimulation::PlaceBuilder placeBuilder;
		auto places = placeBuilder.CreatePlaces(evenCount, country);
		auto population1 = populator1.CreatePopulation(country, places.homes, places.workplaces, places.schools);

		ASSERT_EQ(population1.size(), evenCount);
	}
	TEST_F(PersonPopulatorTest, SizeIsEqualEvenDistributionUnevenCount)
	{
		// Setup population
		DiseaseSpreadSimulation::PlaceBuilder placeBuilder;
		auto places = placeBuilder.CreatePlaces(unevenCount, country);
		auto population2 = populator2.CreatePopulation(country, places.homes, places.workplaces, places.schools);

		ASSERT_EQ(population2.size(), unevenCount);
	}
	TEST_F(PersonPopulatorTest, SizeIsEqualUnevenDistributionEvenCount)
	{
		// Setup population
		DiseaseSpreadSimulation::PlaceBuilder placeBuilder;
		auto places = placeBuilder.CreatePlaces(evenCount, country);
		auto population3 = populator3.CreatePopulation(country, places.homes, places.workplaces, places.schools);

		ASSERT_EQ(population3.size(), evenCount);
	}
	TEST_F(PersonPopulatorTest, SizeIsEqualUnevenDistributionUnevenCount)
	{
		// Setup population
		DiseaseSpreadSimulation::PlaceBuilder placeBuilder;
		auto places = placeBuilder.CreatePlaces(unevenCount, country);
		auto population4 = populator4.CreatePopulation(country, places.homes, places.workplaces, places.schools);

		ASSERT_EQ(population4.size(), unevenCount);
	}
	TEST_F(PersonPopulatorTest, EvenDistributionEvenCount)
	{
		float countHumanDistribution1 = 0.f;
		float countHumanDistribution2 = 0.f;
		float countHumanDistribution3 = 0.f;
		float countHumanDistribution4 = 0.f;

		// Setup population
		DiseaseSpreadSimulation::PlaceBuilder placeBuilder;
		auto places = placeBuilder.CreatePlaces(evenCount, country);
		auto population1 = populator1.CreatePopulation(country, places.homes, places.workplaces, places.schools);

		for (const auto& person : population1)
		{
			DiseaseSpreadSimulation::Statistics::HumanDistribution h{person.GetAgeGroup(), person.GetSex(), 0.f};

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

		// Setup population
		DiseaseSpreadSimulation::PlaceBuilder placeBuilder;
		auto places = placeBuilder.CreatePlaces(unevenCount, country);
		auto population2 = populator2.CreatePopulation(country, places.homes, places.workplaces, places.schools);

		for (const auto& person : population2)
		{
			DiseaseSpreadSimulation::Statistics::HumanDistribution h{person.GetAgeGroup(), person.GetSex(), 0.f};

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

		// Setup population
		DiseaseSpreadSimulation::PlaceBuilder placeBuilder;
		auto places = placeBuilder.CreatePlaces(evenCount, country);
		auto population3 = populator3.CreatePopulation(country, places.homes, places.workplaces, places.schools);

		for (const auto& person : population3)
		{
			DiseaseSpreadSimulation::Statistics::HumanDistribution h{person.GetAgeGroup(), person.GetSex(), 0.f};

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

		// Setup population
		DiseaseSpreadSimulation::PlaceBuilder placeBuilder;
		auto places = placeBuilder.CreatePlaces(unevenCount, country);
		auto population4 = populator4.CreatePopulation(country, places.homes, places.workplaces, places.schools);

		for (const auto& person : population4)
		{
			DiseaseSpreadSimulation::Statistics::HumanDistribution h{person.GetAgeGroup(), person.GetSex(), 0.f};

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
	std::array<float, 4> GetHomePercentFromPopulation(std::vector<DiseaseSpreadSimulation::Person>& population, DiseaseSpreadSimulation::Country country)
	{
		std::map<uint32_t, DiseaseSpreadSimulation::Home*> homesByID;

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
		using namespace DiseaseSpreadSimulation;
		constexpr auto country = Country::USA;
		const std::array<float, 4> distributionArray{PersonPopulator::GetHouseholdDistribution(country).oneMember,
			PersonPopulator::GetHouseholdDistribution(country).twoToThreeMembers,
			PersonPopulator::GetHouseholdDistribution(country).fourToFiveMembers,
			PersonPopulator::GetHouseholdDistribution(country).sixPlusMembers};
		std::shared_mutex distributionArrayMutex;

		constexpr size_t populationSize1 = 1000;
		constexpr size_t testSize = 20;
		std::vector<std::thread> threads;
		threads.reserve(testSize);
		for (size_t j = 0; j < testSize; j++)
		{
			threads.emplace_back([&]()
				{
					PlaceBuilder placeFactory;
					PersonPopulator populationFactory(populationSize1, PersonPopulator::GetCountryDistribution(country));
					auto places = placeFactory.CreatePlaces(populationSize1, country);
					auto population = populationFactory.CreatePopulation(country, places.homes, places.workplaces, places.schools);

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
		auto population = populationFactory.CreatePopulation(country, places.homes, places.workplaces, places.schools);

		auto homePercent2 = GetHomePercentFromPopulation(population, country);
		for (size_t i = 0; i < homePercent2.size(); i++)
		{
			EXPECT_NEAR(homePercent2.at(i), distributionArray.at(i), 0.13f);
		}
	}
	// Helper to sum up all working people
	size_t SumWorkingPeople(size_t populationSize, const DiseaseSpreadSimulation::Country& country)
	{
		using namespace DiseaseSpreadSimulation;
		auto countryDistribution = std::move(PersonPopulator::GetCountryDistribution(country));
		size_t workingPeople{};

		// For every human distribution in country distribution...
		for (const auto& humanDistribution : countryDistribution)
		{
			// ...check if the distribution is inside working age...
			if (humanDistribution.ageGroup > Age_Group::UnderTwenty && humanDistribution.ageGroup <= Age_Group::UnderSeventy)
			{
				// ...and if it is, sum the rounded population size with the distribution applied
				workingPeople += std::llround(populationSize * static_cast<double>(humanDistribution.percent));
			}
		}
		return workingPeople;
	}
	TEST_F(PersonPopulatorTest, WorkingPeopleCount)
	{
		using namespace DiseaseSpreadSimulation;
		size_t populationSize{100};
		EXPECT_EQ(PersonPopulator::WorkingPeopleCount(populationSize, country), SumWorkingPeople(populationSize, country));

		populationSize = 1000;
		EXPECT_EQ(PersonPopulator::WorkingPeopleCount(populationSize, country), SumWorkingPeople(populationSize, country));

		populationSize = 3333;
		EXPECT_EQ(PersonPopulator::WorkingPeopleCount(populationSize, country), SumWorkingPeople(populationSize, country));

		populationSize = 7777;
		EXPECT_EQ(PersonPopulator::WorkingPeopleCount(populationSize, country), SumWorkingPeople(populationSize, country));
	}
} // namespace UnitTests
