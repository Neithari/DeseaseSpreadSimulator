#include "pch.h"
#include "Places/Places.h"
#include "IDGenerator/IDGenerator.h"

uint32_t DeseaseSpreadSimulation::Place::GetID() const
{
	return placeID;
}

void DeseaseSpreadSimulation::Place::AddPerson(Person* person)
{
	std::lock_guard<std::mutex> lockPeople(peopleMutex);
	people.push_back(person);
}

void DeseaseSpreadSimulation::Place::RemovePerson(uint32_t id)
{
	std::lock_guard<std::mutex> lockPeople(peopleMutex);
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

DeseaseSpreadSimulation::Place::Place(const Place& other)
	:
	placeID(other.placeID),
	people(other.people)
{
}

DeseaseSpreadSimulation::Place::Place(Place&& other) noexcept
	:
	placeID(std::move(other.placeID)),
	people(std::move(other.people))
{
}

DeseaseSpreadSimulation::Place& DeseaseSpreadSimulation::Place::operator=(Place&& other) noexcept
{
	std::swap(placeID, other.placeID);
	std::swap(people, other.people);
	return *this;
}

DeseaseSpreadSimulation::Home::Home()
	:
	Place(IDGenerator::IDGenerator<Home>::GetNextID())
{
}

DeseaseSpreadSimulation::Home::Home(const Home& other)
	:
	Place(other)
{
}

DeseaseSpreadSimulation::Home::Home(Home&& other) noexcept
	:
	Place(std::move(other))
{
}

DeseaseSpreadSimulation::Home& DeseaseSpreadSimulation::Home::operator=(const Home& other)
{
	return *this = Home(other);
}

DeseaseSpreadSimulation::Home& DeseaseSpreadSimulation::Home::operator=(Home&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
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

DeseaseSpreadSimulation::Supply::Supply(const Supply& other)
	:
	Place(other)
{
}

DeseaseSpreadSimulation::Supply::Supply(Supply&& other) noexcept
	:
	Place(std::move(other))
{
}

DeseaseSpreadSimulation::Supply& DeseaseSpreadSimulation::Supply::operator=(const Supply& other)
{
	return *this = Supply(other);
}

DeseaseSpreadSimulation::Supply& DeseaseSpreadSimulation::Supply::operator=(Supply&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
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

DeseaseSpreadSimulation::Workplace::Workplace(const Workplace& other)
	:
	Place(other)
{
}

DeseaseSpreadSimulation::Workplace::Workplace(Workplace&& other) noexcept
	:
	Place(std::move(other))
{
}

DeseaseSpreadSimulation::Workplace& DeseaseSpreadSimulation::Workplace::operator=(const Workplace& other)
{
	return *this = Workplace(other);
}

DeseaseSpreadSimulation::Workplace& DeseaseSpreadSimulation::Workplace::operator=(Workplace&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
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

DeseaseSpreadSimulation::HardwareStore::HardwareStore(const HardwareStore& other)
	:
	Place(other)
{
}

DeseaseSpreadSimulation::HardwareStore::HardwareStore(HardwareStore&& other) noexcept
	:
	Place(std::move(other))
{
}

DeseaseSpreadSimulation::HardwareStore& DeseaseSpreadSimulation::HardwareStore::operator=(const HardwareStore& other)
{
	return *this = HardwareStore(other);
}

DeseaseSpreadSimulation::HardwareStore& DeseaseSpreadSimulation::HardwareStore::operator=(HardwareStore&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
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

DeseaseSpreadSimulation::Morgue::Morgue(const Morgue& other)
	:
	Place(other)
{
}

DeseaseSpreadSimulation::Morgue::Morgue(Morgue&& other) noexcept
	:
	Place(std::move(other))
{
}

DeseaseSpreadSimulation::Morgue& DeseaseSpreadSimulation::Morgue::operator=(const Morgue& other)
{
	return *this = Morgue(other);
}

DeseaseSpreadSimulation::Morgue& DeseaseSpreadSimulation::Morgue::operator=(Morgue&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
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

DeseaseSpreadSimulation::School::School(const School& other)
	:
	Place(other)
{
}

DeseaseSpreadSimulation::School::School(School&& other) noexcept
	:
	Place(std::move(other))
{
}

DeseaseSpreadSimulation::School& DeseaseSpreadSimulation::School::operator=(const School& other)
{
	return *this = School(other);
}

DeseaseSpreadSimulation::School& DeseaseSpreadSimulation::School::operator=(School&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
}

DeseaseSpreadSimulation::Place_Type DeseaseSpreadSimulation::School::GetType() const
{
	return Place_Type::School;
}

DeseaseSpreadSimulation::Travel::Travel()
	:
	Place(IDGenerator::IDGenerator<Travel>::GetNextID())
{
}

DeseaseSpreadSimulation::Travel::Travel(const Travel& other)
	:
	Place(other)
{
}

DeseaseSpreadSimulation::Travel::Travel(Travel&& other) noexcept
	:
	Place(std::move(other))
{
}

DeseaseSpreadSimulation::Travel& DeseaseSpreadSimulation::Travel::operator=(const Travel& other)
{
	return *this = Travel(other);
}

DeseaseSpreadSimulation::Travel& DeseaseSpreadSimulation::Travel::operator=(Travel&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
}

DeseaseSpreadSimulation::Place_Type DeseaseSpreadSimulation::Travel::GetType() const
{
	return Place_Type::Travel;
}

void DeseaseSpreadSimulation::Places::Insert(Places other)
{
	homes.reserve(homes.size() + other.homes.size());
	AppendVectorAtEnd(homes, other.homes);
	
	supplyStores.reserve(supplyStores.size() + other.supplyStores.size());
	AppendVectorAtEnd(supplyStores, other.supplyStores);
	
	workplaces.reserve(workplaces.size() + other.workplaces.size());
	AppendVectorAtEnd(workplaces, other.workplaces);
	
	schools.reserve(schools.size() + other.schools.size());
	AppendVectorAtEnd(schools, other.schools);
	
	hardwareStores.reserve(hardwareStores.size() + other.hardwareStores.size());
	AppendVectorAtEnd(hardwareStores, other.hardwareStores);
	
	morgues.reserve(morgues.size() + other.morgues.size());
	AppendVectorAtEnd(morgues, other.morgues);
}
