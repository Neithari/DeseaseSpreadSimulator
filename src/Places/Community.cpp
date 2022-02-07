#include "pch.h"
#include "Places/Community.h"
#include <gsl/gsl>

DeseaseSpreadSimulation::Community::Community(std::vector<Person> population, Places places)
	:
	m_population(std::move(population)),
	m_places(std::move(places))
{
}

DeseaseSpreadSimulation::Community::Community(const Community& other)
	:
	m_population(other.m_population),
	m_places(other.m_places)
{
}

DeseaseSpreadSimulation::Community::Community(Community&& other)
	:
	m_population(other.m_population),
	m_places(other.m_places)
{
}

DeseaseSpreadSimulation::Community& DeseaseSpreadSimulation::Community::operator=(const Community& other)
{
	return *this = Community(other);
}

DeseaseSpreadSimulation::Community& DeseaseSpreadSimulation::Community::operator=(Community&& other)
{
	std::swap(m_population, other.m_population);
	std::swap(m_places, other.m_places);
	return *this;
}

void DeseaseSpreadSimulation::Community::AddPlace(Home home)
{
	m_places.homes.push_back(home);
}

void DeseaseSpreadSimulation::Community::AddPlace(Supply store)
{
	m_places.supplyStores.push_back(store);
}

void DeseaseSpreadSimulation::Community::AddPlace(Workplace workplace)
{
	m_places.workplaces.push_back(workplace);
}

void DeseaseSpreadSimulation::Community::AddPlace(School school)
{
	m_places.schools.push_back(school);
}

void DeseaseSpreadSimulation::Community::AddPlace(HardwareStore store)
{
	m_places.hardwareStores.push_back(store);
}

void DeseaseSpreadSimulation::Community::AddPlace(Morgue morgue)
{
	m_places.morgues.push_back(morgue);
}

void DeseaseSpreadSimulation::Community::AddPlaces(Places places)
{
	m_places.Insert(places);
}

void DeseaseSpreadSimulation::Community::AddPerson(Person person)
{
	person.SetCommunity(this);
	m_population.push_back(std::move(person));
}

void DeseaseSpreadSimulation::Community::AddPopulation(std::vector<Person>& population)
{
	m_population.reserve(m_population.size() + population.size());
	m_population.insert(m_population.end(), population.begin(), population.end());
}

void DeseaseSpreadSimulation::Community::RemovePerson(uint32_t personID)
{
	m_population.erase(
		std::remove_if(m_population.begin(), m_population.end(),
			[&](const Person& person) { return person.GetID() == personID; }), m_population.end()
	);
}

DeseaseSpreadSimulation::Person DeseaseSpreadSimulation::Community::TransferPerson(uint32_t personID)
{
	auto toTransfer = std::find_if(m_population.begin(), m_population.end(),
		[&](const Person& person) { return person.GetID() == personID; });
	
	if (toTransfer != m_population.end())
	{
		auto transferPerson = std::move(*toTransfer);
		m_population.erase(toTransfer);
		return transferPerson;
	}
	// This should never happen, because the person to transfer is calling it.
	// So we throw and silence compiler warnings
	throw(std::invalid_argument("person not found"));
	GSL_ASSUME(false);
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToPlace(Person* person, Place* place)
{
	person->GetWhereabouts()->RemovePerson(person);
	place->AddPerson(person);
	return place;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToHome(Person* person)
{
	std::lock_guard<std::mutex> lockHomes(mutexHome);
	auto home = person->GetHome();
	TransferToPlace(person, home);
	return home;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToSupplyStore(Person* person)
{
	std::lock_guard<std::mutex> lockSupplyStores(mutexSupply);
	auto store = GetSupplyStore();
	TransferToPlace(person, store);
	return store;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToHardwareStore(Person* person)
{
	std::lock_guard<std::mutex> lockHardwareStores(mutexHardware);
	auto store = GetHardwareStore();
	TransferToPlace(person, store);
	return store;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToWork(Person* person)
{
	std::lock_guard<std::mutex> lockWorkplaces(mutexWork);
	auto work = person->GetWorkplace();
	TransferToPlace(person, work);
	return work;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToSchool(Person* person)
{
	std::lock_guard<std::mutex> lockSchools(mutexSchool);
	auto school = person->GetSchool();
	TransferToPlace(person, school);
	return school;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToMorgue(Person* person)
{
	std::lock_guard<std::mutex> lockMorgues(mutexMorgue);
	auto morgue = GetMorgue();
	TransferToPlace(person, morgue);
	return morgue;
}

std::vector<DeseaseSpreadSimulation::Person>& DeseaseSpreadSimulation::Community::GetPopulation()
{
	return m_population;
}

DeseaseSpreadSimulation::Places& DeseaseSpreadSimulation::Community::GetPlaces()
{
	return m_places;
}

std::vector<DeseaseSpreadSimulation::Home>& DeseaseSpreadSimulation::Community::GetHomes()
{
	return m_places.homes;
}

DeseaseSpreadSimulation::Supply* DeseaseSpreadSimulation::Community::GetSupplyStore()
{
	if (m_places.supplyStores.empty())
	{
		return nullptr;
	}

	return &m_places.supplyStores.at(RandomIndex(m_places.supplyStores.size()));
}

DeseaseSpreadSimulation::HardwareStore* DeseaseSpreadSimulation::Community::GetHardwareStore()
{
	if (m_places.hardwareStores.empty())
	{
		return nullptr;
	}

	return &m_places.hardwareStores.at(RandomIndex(m_places.hardwareStores.size()));
}

DeseaseSpreadSimulation::Morgue* DeseaseSpreadSimulation::Community::GetMorgue()
{
	if (m_places.morgues.empty())
	{
		return nullptr;
	}

	return &m_places.morgues.at(RandomIndex(m_places.morgues.size()));
}
