#include "Places/Places.h"
#include "Enums.h"
#include "IDGenerator/IDGenerator.h"
#include "Person/Person.h"

uint32_t DiseaseSpreadSimulation::Place::GetID() const
{
	return placeID;
}

void DiseaseSpreadSimulation::Place::AddPerson(Person* person)
{
	std::lock_guard<std::mutex> lockPeople(peopleMutex);
	people.push_back(person);
}

void DiseaseSpreadSimulation::Place::RemovePerson(uint32_t id)
{
	std::lock_guard<std::mutex> lockPeople(peopleMutex);
	people.erase(
		std::remove_if(people.begin(), people.end(), [&](Person* person)
			{
				return person->GetID() == id;
			}),
		people.end());
}

void DiseaseSpreadSimulation::Place::RemovePerson(Person* person)
{
	RemovePerson(person->GetID());
}

std::string DiseaseSpreadSimulation::Place::TypeToString(Place_Type type)
{
	switch (type)
	{
	case Place_Type::Home:
		return "Home";
		break;
	case Place_Type::Supply:
		return "Supply Store";
		break;
	case Place_Type::Workplace:
		return "Workplace";
		break;
	case Place_Type::School:
		return "School";
		break;
	case Place_Type::HardwareStore:
		return "Hardware Store";
		break;
	case Place_Type::Morgue:
		return "Morgue";
		break;
	case Place_Type::Travel:
		return "Travel location";
		break;
	default:
		break;
	}
	return std::string();
}

std::vector<DiseaseSpreadSimulation::Person*>& DiseaseSpreadSimulation::Place::GetPeople()
{
	return people;
}

size_t DiseaseSpreadSimulation::Place::GetPersonCount() const
{
	return people.size();
}

DiseaseSpreadSimulation::Place::Place(uint32_t id)
	: placeID(id)
{
}

DiseaseSpreadSimulation::Place::Place(const Place& other)
	: placeID(other.placeID),
	  people(other.people),
	  peopleMutex()
{
}

DiseaseSpreadSimulation::Place::Place(Place&& other) noexcept
	: placeID(std::move(other.placeID)),
	  people(std::move(other.people)),
	  peopleMutex()
{
}

// We don't want to copy peopleMutex so we suppress the static analyzer warning
// cppcheck-suppress operatorEqVarError
DiseaseSpreadSimulation::Place& DiseaseSpreadSimulation::Place::operator=(Place&& other) noexcept
{
	std::swap(placeID, other.placeID);
	std::swap(people, other.people);
	return *this;
}

DiseaseSpreadSimulation::Home::Home()
	: Place(IDGenerator::IDGenerator<Home>::GetNextID())
{
}

DiseaseSpreadSimulation::Home::Home(const Home& other)
	: Place(other)
{
}

DiseaseSpreadSimulation::Home::Home(Home&& other) noexcept
	: Place(std::move(other))
{
}

DiseaseSpreadSimulation::Home& DiseaseSpreadSimulation::Home::operator=(const Home& other)
{
	return *this = Home(other);
}

DiseaseSpreadSimulation::Home& DiseaseSpreadSimulation::Home::operator=(Home&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
}

DiseaseSpreadSimulation::Place_Type DiseaseSpreadSimulation::Home::GetType() const
{
	return Place_Type::Home;
}

DiseaseSpreadSimulation::Supply::Supply()
	: Place(IDGenerator::IDGenerator<Supply>::GetNextID())
{
}

DiseaseSpreadSimulation::Supply::Supply(const Supply& other)
	: Place(other)
{
}

DiseaseSpreadSimulation::Supply::Supply(Supply&& other) noexcept
	: Place(std::move(other))
{
}

DiseaseSpreadSimulation::Supply& DiseaseSpreadSimulation::Supply::operator=(const Supply& other)
{
	return *this = Supply(other);
}

DiseaseSpreadSimulation::Supply& DiseaseSpreadSimulation::Supply::operator=(Supply&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
}

DiseaseSpreadSimulation::Place_Type DiseaseSpreadSimulation::Supply::GetType() const
{
	return Place_Type::Supply;
}

DiseaseSpreadSimulation::Workplace::Workplace()
	: Place(IDGenerator::IDGenerator<Workplace>::GetNextID())
{
}

DiseaseSpreadSimulation::Workplace::Workplace(const Workplace& other)
	: Place(other)
{
}

DiseaseSpreadSimulation::Workplace::Workplace(Workplace&& other) noexcept
	: Place(std::move(other))
{
}

DiseaseSpreadSimulation::Workplace& DiseaseSpreadSimulation::Workplace::operator=(const Workplace& other)
{
	return *this = Workplace(other);
}

DiseaseSpreadSimulation::Workplace& DiseaseSpreadSimulation::Workplace::operator=(Workplace&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
}

DiseaseSpreadSimulation::Place_Type DiseaseSpreadSimulation::Workplace::GetType() const
{
	return Place_Type::Workplace;
}

DiseaseSpreadSimulation::HardwareStore::HardwareStore()
	: Place(IDGenerator::IDGenerator<HardwareStore>::GetNextID())
{
}

DiseaseSpreadSimulation::HardwareStore::HardwareStore(const HardwareStore& other)
	: Place(other)
{
}

DiseaseSpreadSimulation::HardwareStore::HardwareStore(HardwareStore&& other) noexcept
	: Place(std::move(other))
{
}

DiseaseSpreadSimulation::HardwareStore& DiseaseSpreadSimulation::HardwareStore::operator=(const HardwareStore& other)
{
	return *this = HardwareStore(other);
}

DiseaseSpreadSimulation::HardwareStore& DiseaseSpreadSimulation::HardwareStore::operator=(HardwareStore&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
}

DiseaseSpreadSimulation::Place_Type DiseaseSpreadSimulation::HardwareStore::GetType() const
{
	return Place_Type::HardwareStore;
}

DiseaseSpreadSimulation::Morgue::Morgue()
	: Place(IDGenerator::IDGenerator<Morgue>::GetNextID())
{
}

DiseaseSpreadSimulation::Morgue::Morgue(const Morgue& other)
	: Place(other)
{
}

DiseaseSpreadSimulation::Morgue::Morgue(Morgue&& other) noexcept
	: Place(std::move(other))
{
}

DiseaseSpreadSimulation::Morgue& DiseaseSpreadSimulation::Morgue::operator=(const Morgue& other)
{
	return *this = Morgue(other);
}

DiseaseSpreadSimulation::Morgue& DiseaseSpreadSimulation::Morgue::operator=(Morgue&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
}

DiseaseSpreadSimulation::Place_Type DiseaseSpreadSimulation::Morgue::GetType() const
{
	return Place_Type::Morgue;
}

DiseaseSpreadSimulation::School::School()
	: Place(IDGenerator::IDGenerator<School>::GetNextID())
{
}

DiseaseSpreadSimulation::School::School(const School& other)
	: Place(other)
{
}

DiseaseSpreadSimulation::School::School(School&& other) noexcept
	: Place(std::move(other))
{
}

DiseaseSpreadSimulation::School& DiseaseSpreadSimulation::School::operator=(const School& other)
{
	return *this = School(other);
}

DiseaseSpreadSimulation::School& DiseaseSpreadSimulation::School::operator=(School&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
}

DiseaseSpreadSimulation::Place_Type DiseaseSpreadSimulation::School::GetType() const
{
	return Place_Type::School;
}

DiseaseSpreadSimulation::Travel::Travel()
	: Place(IDGenerator::IDGenerator<Travel>::GetNextID())
{
}

DiseaseSpreadSimulation::Travel::Travel(const Travel& other)
	: Place(other)
{
}

DiseaseSpreadSimulation::Travel::Travel(Travel&& other) noexcept
	: Place(std::move(other))
{
}

DiseaseSpreadSimulation::Travel& DiseaseSpreadSimulation::Travel::operator=(const Travel& other)
{
	return *this = Travel(other);
}

DiseaseSpreadSimulation::Travel& DiseaseSpreadSimulation::Travel::operator=(Travel&& other) noexcept
{
	Place::operator=(std::move(other));
	return *this;
}

DiseaseSpreadSimulation::Place_Type DiseaseSpreadSimulation::Travel::GetType() const
{
	return Place_Type::Travel;
}

void DiseaseSpreadSimulation::Places::Insert(Places other)
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
