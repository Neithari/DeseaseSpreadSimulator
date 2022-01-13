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

const std::vector<DeseaseSpreadSimulation::Person*>& DeseaseSpreadSimulation::Place::GetPeople() const
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
