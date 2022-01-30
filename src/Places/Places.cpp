#include "pch.h"
#include "Places/Places.h"
#include "IDGenerator/IDGenerator.h"

uint32_t DeseaseSpreadSimulation::Place::GetID() const
{
	return placeID;
}

void DeseaseSpreadSimulation::Place::AddPerson(Person* person)
{
	people.push_back(person);
}

void DeseaseSpreadSimulation::Place::RemovePerson(uint32_t id)
{
	people.erase(
		std::remove_if(people.begin(), people.end(),
			[&](Person* person) { return person->GetID() == id; }), people.end()
	);
}

void DeseaseSpreadSimulation::Place::RemovePerson(Person* person)
{
	RemovePerson(person->GetID());
}

std::string DeseaseSpreadSimulation::Place::TypeToString(Place_Type type)
{
	switch (type)
	{
	case DeseaseSpreadSimulation::Place_Type::Home:
		return "Home";
		break;
	case DeseaseSpreadSimulation::Place_Type::Supply:
		return "Supply Store";
		break;
	case DeseaseSpreadSimulation::Place_Type::Workplace:
		return "Workplace";
		break;
	case DeseaseSpreadSimulation::Place_Type::School:
		return "School";
		break;
	case DeseaseSpreadSimulation::Place_Type::HardwareStore:
		return "Hardware Store";
		break;
	case DeseaseSpreadSimulation::Place_Type::Morgue:
		return "Morgue";
		break;
	default:
		break;
	}
	return std::string();
}

std::vector<DeseaseSpreadSimulation::Person*>& DeseaseSpreadSimulation::Place::GetPeople()
{
	return people;
}

size_t DeseaseSpreadSimulation::Place::GetPersonCount() const
{
	return people.size();
}

DeseaseSpreadSimulation::Place::Place(uint32_t id)
	:
	placeID(id)
{
}

DeseaseSpreadSimulation::Home::Home()
	:
	Place(IDGenerator::IDGenerator<Home>::GetNextID())
{
}

DeseaseSpreadSimulation::Place_Type DeseaseSpreadSimulation::Home::GetType() const
{
	return Place_Type::Home;
}

DeseaseSpreadSimulation::Supply::Supply()
	:
	Place(IDGenerator::IDGenerator<Supply>::GetNextID())
{
}

DeseaseSpreadSimulation::Place_Type DeseaseSpreadSimulation::Supply::GetType() const
{
	return Place_Type::Supply;
}

DeseaseSpreadSimulation::Workplace::Workplace()
	:
	Place(IDGenerator::IDGenerator<Workplace>::GetNextID())
{
}

DeseaseSpreadSimulation::Place_Type DeseaseSpreadSimulation::Workplace::GetType() const
{
	return Place_Type::Workplace;
}

DeseaseSpreadSimulation::HardwareStore::HardwareStore()
	:
	Place(IDGenerator::IDGenerator<HardwareStore>::GetNextID())
{
}

DeseaseSpreadSimulation::Place_Type DeseaseSpreadSimulation::HardwareStore::GetType() const
{
	return Place_Type::HardwareStore;
}

DeseaseSpreadSimulation::Morgue::Morgue()
	:
	Place(IDGenerator::IDGenerator<Morgue>::GetNextID())
{
}

DeseaseSpreadSimulation::Place_Type DeseaseSpreadSimulation::Morgue::GetType() const
{
	return Place_Type::Morgue;
}

DeseaseSpreadSimulation::School::School()
	:
	Place(IDGenerator::IDGenerator<School>::GetNextID())
{
}

DeseaseSpreadSimulation::Place_Type DeseaseSpreadSimulation::School::GetType() const
{
	return Place_Type::School;
}

void DeseaseSpreadSimulation::Places::Insert(Places other)
{
	homes.reserve(homes.size() + other.homes.size());
	homes.insert(homes.end(), other.homes.begin(), other.homes.end());

	supplyStores.reserve(supplyStores.size() + other.supplyStores.size());
	supplyStores.insert(supplyStores.end(), other.supplyStores.begin(), other.supplyStores.end());

	workplaces.reserve(workplaces.size() + other.workplaces.size());
	workplaces.insert(workplaces.end(), other.workplaces.begin(), other.workplaces.end());

	schools.reserve(schools.size() + other.schools.size());
	schools.insert(schools.end(), other.schools.begin(), other.schools.end());

	hardwareStores.reserve(hardwareStores.size() + other.hardwareStores.size());
	hardwareStores.insert(hardwareStores.end(), other.hardwareStores.begin(), other.hardwareStores.end());

	morgues.reserve(morgues.size() + other.morgues.size());
	morgues.insert(morgues.end(), other.morgues.begin(), other.morgues.end());
}
