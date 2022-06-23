#include <gtest/gtest.h>
#include <cstdint>
#include <vector>
#include "Enums.h"
#include "Places/Places.h"
#include "Person/Person.h"
#include "Person/PersonBehavior.h"

namespace UnitTests
{
	class PlaceTests : public ::testing::Test
	{
	protected:
		DiseaseSpreadSimulation::Home home;
		DiseaseSpreadSimulation::Supply market;
		DiseaseSpreadSimulation::Workplace work;
		DiseaseSpreadSimulation::School school;
		DiseaseSpreadSimulation::HardwareStore hardware;
		DiseaseSpreadSimulation::Morgue morgue;
		DiseaseSpreadSimulation::Travel travel;
		DiseaseSpreadSimulation::PersonBehavior behavior;
	};
	TEST_F(PlaceTests, GetType)
	{
		EXPECT_EQ(home.GetType(), DiseaseSpreadSimulation::Place_Type::Home);
		EXPECT_EQ(market.GetType(), DiseaseSpreadSimulation::Place_Type::Supply);
		EXPECT_EQ(work.GetType(), DiseaseSpreadSimulation::Place_Type::Workplace);
		EXPECT_EQ(school.GetType(), DiseaseSpreadSimulation::Place_Type::School);
		EXPECT_EQ(hardware.GetType(), DiseaseSpreadSimulation::Place_Type::HardwareStore);
		EXPECT_EQ(morgue.GetType(), DiseaseSpreadSimulation::Place_Type::Morgue);
		EXPECT_EQ(travel.GetType(), DiseaseSpreadSimulation::Place_Type::Travel);
	}
	TEST_F(PlaceTests, AddPerson)
	{
		DiseaseSpreadSimulation::Person person(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, nullptr);
		auto personID = person.GetID();
		ASSERT_EQ(home.GetPersonCount(), 0);
		home.AddPerson(&person);
		ASSERT_EQ(home.GetPersonCount(), 1);
		ASSERT_EQ(home.GetPeople().back()->GetID(), personID);
	}
	TEST_F(PlaceTests, GetPersonCount)
	{
		DiseaseSpreadSimulation::Person person(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, nullptr);
		DiseaseSpreadSimulation::Person person1(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Female, behavior, nullptr);
		DiseaseSpreadSimulation::Person person2(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Female, behavior, nullptr);

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
		DiseaseSpreadSimulation::Person person(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, nullptr);
		DiseaseSpreadSimulation::Person person1(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Male, behavior, nullptr);
		DiseaseSpreadSimulation::Person person2(DiseaseSpreadSimulation::Age_Group::UnderTwenty, DiseaseSpreadSimulation::Sex::Female, behavior, nullptr);
		auto personID = person.GetID();
		auto personID1 = person1.GetID();
		auto personID2 = person2.GetID();

		home.AddPerson(&person);
		home.AddPerson(&person1);
		home.AddPerson(&person2);
		ASSERT_EQ(home.GetPersonCount(), 3);

		// Check that a wrong ID will not remove a valid person
		home.RemovePerson(12345); // NOLINT(*-magic-numbers)
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
		home.RemovePerson(&person2);
		ASSERT_EQ(home.GetPersonCount(), 0);
	}
	TEST_F(PlaceTests, TypeToString)
	{
		using namespace DiseaseSpreadSimulation;

		EXPECT_EQ(home.TypeToString(home.GetType()), "Home");
		EXPECT_EQ(market.TypeToString(market.GetType()), "Supply Store");
		EXPECT_EQ(work.TypeToString(work.GetType()), "Workplace");
		EXPECT_EQ(school.TypeToString(school.GetType()), "School");
		EXPECT_EQ(hardware.TypeToString(hardware.GetType()), "Hardware Store");
		EXPECT_EQ(morgue.TypeToString(morgue.GetType()), "Morgue");
		EXPECT_EQ(travel.TypeToString(travel.GetType()), "Travel location");
	}

	TEST(PlacesTests, InsertPlaces)
	{
		using namespace DiseaseSpreadSimulation;

		Places places1{};
		Places places2{};

		places2.homes.emplace_back(Home{});
		places2.supplyStores.emplace_back(Supply{});
		places2.workplaces.emplace_back(Workplace{});
		places2.schools.emplace_back(School{});
		places2.hardwareStores.emplace_back(HardwareStore{});
		places2.morgues.emplace_back(Morgue{});

		// Inserting into empty vector
		places1.Insert(places2);
		ASSERT_EQ(places1.homes.size(), places2.homes.size());
		ASSERT_EQ(places1.supplyStores.size(), places2.supplyStores.size());
		ASSERT_EQ(places1.workplaces.size(), places2.workplaces.size());
		ASSERT_EQ(places1.schools.size(), places2.schools.size());
		ASSERT_EQ(places1.hardwareStores.size(), places2.hardwareStores.size());
		ASSERT_EQ(places1.morgues.size(), places2.morgues.size());

		EXPECT_EQ(places1.homes.back().GetID(), places2.homes.back().GetID());
		EXPECT_EQ(places1.supplyStores.back().GetID(), places2.supplyStores.back().GetID());
		EXPECT_EQ(places1.workplaces.back().GetID(), places2.workplaces.back().GetID());
		EXPECT_EQ(places1.schools.back().GetID(), places2.schools.back().GetID());
		EXPECT_EQ(places1.hardwareStores.back().GetID(), places2.hardwareStores.back().GetID());
		EXPECT_EQ(places1.morgues.back().GetID(), places2.morgues.back().GetID());

		Places places3{};
		places3.homes.emplace_back(Home{});
		places3.homes.emplace_back(Home{});
		places3.supplyStores.emplace_back(Supply{});
		places3.supplyStores.emplace_back(Supply{});
		places3.workplaces.emplace_back(Workplace{});
		places3.workplaces.emplace_back(Workplace{});
		places3.schools.emplace_back(School{});
		places3.schools.emplace_back(School{});
		places3.hardwareStores.emplace_back(HardwareStore{});
		places3.hardwareStores.emplace_back(HardwareStore{});
		places3.morgues.emplace_back(Morgue{});
		places3.morgues.emplace_back(Morgue{});

		// Inserting into vector with contend
		places1.Insert(places3);
		ASSERT_EQ(places1.homes.size(), places2.homes.size() + places3.homes.size());
		ASSERT_EQ(places1.supplyStores.size(), places2.supplyStores.size() + places3.supplyStores.size());
		ASSERT_EQ(places1.workplaces.size(), places2.workplaces.size() + places3.workplaces.size());
		ASSERT_EQ(places1.schools.size(), places2.schools.size() + places3.schools.size());
		ASSERT_EQ(places1.hardwareStores.size(), places2.hardwareStores.size() + places3.hardwareStores.size());
		ASSERT_EQ(places1.morgues.size(), places2.morgues.size() + places3.morgues.size());

		EXPECT_EQ(places1.homes.at(0).GetID(), places2.homes.at(0).GetID());
		EXPECT_EQ(places1.homes.at(1).GetID(), places3.homes.at(1).GetID());
		EXPECT_EQ(places1.homes.at(2).GetID(), places3.homes.at(0).GetID());

		EXPECT_EQ(places1.supplyStores.at(0).GetID(), places2.supplyStores.at(0).GetID());
		EXPECT_EQ(places1.supplyStores.at(1).GetID(), places3.supplyStores.at(1).GetID());
		EXPECT_EQ(places1.supplyStores.at(2).GetID(), places3.supplyStores.at(0).GetID());

		EXPECT_EQ(places1.workplaces.at(0).GetID(), places2.workplaces.at(0).GetID());
		EXPECT_EQ(places1.workplaces.at(1).GetID(), places3.workplaces.at(1).GetID());
		EXPECT_EQ(places1.workplaces.at(2).GetID(), places3.workplaces.at(0).GetID());

		EXPECT_EQ(places1.schools.at(0).GetID(), places2.schools.at(0).GetID());
		EXPECT_EQ(places1.schools.at(1).GetID(), places3.schools.at(1).GetID());
		EXPECT_EQ(places1.schools.at(2).GetID(), places3.schools.at(0).GetID());

		EXPECT_EQ(places1.hardwareStores.at(0).GetID(), places2.hardwareStores.at(0).GetID());
		EXPECT_EQ(places1.hardwareStores.at(1).GetID(), places3.hardwareStores.at(1).GetID());
		EXPECT_EQ(places1.hardwareStores.at(2).GetID(), places3.hardwareStores.at(0).GetID());

		EXPECT_EQ(places1.morgues.at(0).GetID(), places2.morgues.at(0).GetID());
		EXPECT_EQ(places1.morgues.at(1).GetID(), places3.morgues.at(1).GetID());
		EXPECT_EQ(places1.morgues.at(2).GetID(), places3.morgues.at(0).GetID());
	}
} // namespace UnitTests
