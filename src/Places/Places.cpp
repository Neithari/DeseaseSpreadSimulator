#include "pch.h"
#include "Places/Places.h"
#include "Person/Person.h"

unsigned int DeseaseSpreadSimulation::Place::GetID() const
{
	return placeID;
}

const std::vector<DeseaseSpreadSimulation::Person>& DeseaseSpreadSimulation::Place::GetPeople() const
{
	return people;
}

std::unique_ptr<std::vector<DeseaseSpreadSimulation::Person>> DeseaseSpreadSimulation::Place::GetPeople()
{
	return std::make_unique<std::vector<Person>>(people);
}

DeseaseSpreadSimulation::Place::Place(unsigned int id)
	:
	placeID(id)
{
}

DeseaseSpreadSimulation::Home::Home()
	:
	Place(IDGenerator::IDGenerator<Home>::GetNextID())
{
}

std::string DeseaseSpreadSimulation::Home::GetTypeName() const
{
	std::string name = "Home" + std::to_string(GetID());
	return name;
}

DeseaseSpreadSimulation::Supply::Supply()
	:
	Place(IDGenerator::IDGenerator<Supply>::GetNextID())
{
}

std::string DeseaseSpreadSimulation::Supply::GetTypeName() const
{
	return "Supply" + std::to_string(GetID());
}

DeseaseSpreadSimulation::Work::Work()
	:
	Place(IDGenerator::IDGenerator<Work>::GetNextID())
{
}

std::string DeseaseSpreadSimulation::Work::GetTypeName() const
{
	return "Work" + std::to_string(GetID());
}

DeseaseSpreadSimulation::HardwareStore::HardwareStore()
	:
	Place(IDGenerator::IDGenerator<HardwareStore>::GetNextID())
{
}

std::string DeseaseSpreadSimulation::HardwareStore::GetTypeName() const
{
	return "HardwareStore" + std::to_string(GetID());
}