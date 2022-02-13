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
	m_places(other.m_places),
	m_travelLocation(other.m_travelLocation)
{
}

DeseaseSpreadSimulation::Community::Community(Community&& other) noexcept
	:
	m_population(std::move(other.m_population)),
	m_places(std::move(other.m_places)),
	m_travelLocation(std::move(other.m_travelLocation))
{
}

DeseaseSpreadSimulation::Community& DeseaseSpreadSimulation::Community::operator=(const Community& other)
{
	return *this = Community(other);
}

DeseaseSpreadSimulation::Community& DeseaseSpreadSimulation::Community::operator=(Community&& other) noexcept
{
	std::swap(m_population, other.m_population);
	std::swap(m_places, other.m_places);
	std::swap(m_travelLocation, other.m_travelLocation);
	return *this;
}

void DeseaseSpreadSimulation::Community::AddPerson(Person person)
{
	person.SetCommunity(this);
	std::lock_guard<std::shared_timed_mutex> lockAddPerson(populationMutex);
	m_population.push_back(std::move(person));
}

void DeseaseSpreadSimulation::Community::RemovePerson(const Person& personToRemove)
{
	std::lock_guard<std::shared_timed_mutex> lockRemovePerson(populationMutex);
	m_population.erase(
		std::remove_if(m_population.begin(), m_population.end(),
			[&](const Person& person) { return person == personToRemove; }), m_population.end()
	);
}

void DeseaseSpreadSimulation::Community::AddPlaces(Places places)
{
	std::lock_guard<std::shared_timed_mutex> lockAddPlaces(placesMutex);
	m_places.Insert(places);
}

void DeseaseSpreadSimulation::Community::AddPopulation(std::vector<Person>& population)
{
	std::lock_guard<std::shared_timed_mutex> lockAddPopulation(populationMutex);
	m_population.reserve(m_population.size() + population.size());
	m_population.insert(m_population.end(), population.begin(), population.end());
}

std::optional<DeseaseSpreadSimulation::Person> DeseaseSpreadSimulation::Community::TransferPerson(const Person& traveler)
{
	// The shared lock can't be upgraded to a full lock. Because of that we need to separate the read from the write part.
	bool isEnditerator{ true };
	std::vector<Person>::iterator toTransfer;
	{
		std::shared_lock<std::shared_timed_mutex> lockFindPerson(populationMutex);
		toTransfer = std::find_if(m_population.begin(), m_population.end(),
			[&](const Person& person) { return person == traveler; });
		isEnditerator = toTransfer == m_population.end();
	}

	if (!isEnditerator)
	{
		std::lock_guard<std::shared_timed_mutex> lockPersonTransfer(populationMutex);
		std::optional<Person> transferPerson = std::move(*toTransfer);
		m_population.erase(toTransfer);
		return transferPerson;
	}
	// This should never happen, because the person to transfer should be calling it.
	return std::nullopt;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToHome(Person* person)
{
	auto home = person->GetHome();
	TransferToPlace(person, home);
	return home;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToSupplyStore(Person* person)
{
	auto store = GetSupplyStore();
	TransferToPlace(person, store);
	return store;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToHardwareStore(Person* person)
{
	auto store = GetHardwareStore();
	TransferToPlace(person, store);
	return store;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToWork(Person* person)
{
	auto work = person->GetWorkplace();
	TransferToPlace(person, work);
	return work;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToSchool(Person* person)
{
	auto school = person->GetSchool();
	TransferToPlace(person, school);
	return school;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToMorgue(Person* person)
{
	auto morgue = GetMorgue();
	TransferToPlace(person, morgue);
	return morgue;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToTravelLocation(Person* person)
{
	// Currently only one travel location
	TransferToPlace(person, &m_travelLocation);
	return &m_travelLocation;
}

std::vector<DeseaseSpreadSimulation::Person>& DeseaseSpreadSimulation::Community::GetPopulation()
{
	return m_population;
}

DeseaseSpreadSimulation::Places& DeseaseSpreadSimulation::Community::GetPlaces()
{
	return m_places;
}

DeseaseSpreadSimulation::Travel& DeseaseSpreadSimulation::Community::GetTravelLocation()
{
	return m_travelLocation;
}

std::vector<DeseaseSpreadSimulation::Home>& DeseaseSpreadSimulation::Community::GetHomes()
{
	return m_places.homes;
}

DeseaseSpreadSimulation::Supply* DeseaseSpreadSimulation::Community::GetSupplyStore()
{
	std::shared_lock<std::shared_timed_mutex> lockGetPlaces(placesMutex);
	if (m_places.supplyStores.empty())
	{
		return nullptr;
	}

	return &m_places.supplyStores.at(Random::RandomVectorIndex(m_places.supplyStores));
}

DeseaseSpreadSimulation::HardwareStore* DeseaseSpreadSimulation::Community::GetHardwareStore()
{
	std::shared_lock<std::shared_timed_mutex> lockGetPlaces(placesMutex);
	if (m_places.hardwareStores.empty())
	{
		return nullptr;
	}

	return &m_places.hardwareStores.at(Random::RandomVectorIndex(m_places.hardwareStores));
}

DeseaseSpreadSimulation::Morgue* DeseaseSpreadSimulation::Community::GetMorgue()
{
	std::shared_lock<std::shared_timed_mutex> lockGetPlaces(placesMutex);
	if (m_places.morgues.empty())
	{
		return nullptr;
	}
	
	return &m_places.morgues.at(Random::RandomVectorIndex(m_places.morgues));
}

void DeseaseSpreadSimulation::Community::AddPlace(Home home)
{
	std::lock_guard<std::shared_timed_mutex> lockAddPlace(placesMutex);
	m_places.homes.push_back(home);
}

void DeseaseSpreadSimulation::Community::AddPlace(Supply store)
{
	std::lock_guard<std::shared_timed_mutex> lockAddPlace(placesMutex);
	m_places.supplyStores.push_back(store);
}

void DeseaseSpreadSimulation::Community::AddPlace(Workplace workplace)
{
	std::lock_guard<std::shared_timed_mutex> lockAddPlace(placesMutex);
	m_places.workplaces.push_back(workplace);
}

void DeseaseSpreadSimulation::Community::AddPlace(School school)
{
	std::lock_guard<std::shared_timed_mutex> lockAddPlace(placesMutex);
	m_places.schools.push_back(school);
}

void DeseaseSpreadSimulation::Community::AddPlace(HardwareStore store)
{
	std::lock_guard<std::shared_timed_mutex> lockAddPlace(placesMutex);
	m_places.hardwareStores.push_back(store);
}

void DeseaseSpreadSimulation::Community::AddPlace(Morgue morgue)
{
	std::lock_guard<std::shared_timed_mutex> lockAddPlace(placesMutex);
	m_places.morgues.push_back(morgue);
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToPlace(Person* person, Place* place)
{
	std::lock_guard<std::shared_timed_mutex> lockTransferToPlace(placesMutex);
	person->GetWhereabouts()->RemovePerson(person);
	place->AddPerson(person);
	return place;
}
