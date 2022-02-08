#include "pch.h"
#include "Places/Community.h"
#include <gsl/gsl>

DeseaseSpreadSimulation::Community::Community(std::vector<Person> population, Places places)
	:
	m_population(std::move(population)),
	m_places(std::move(places))
{
}

void DeseaseSpreadSimulation::Community::AddPerson(Person person)
{
	person.SetCommunity(this);
	m_population.push_back(std::move(person));
}

void DeseaseSpreadSimulation::Community::RemovePerson(const Person& person)
{
	RemovePerson(person.GetID());
}

void DeseaseSpreadSimulation::Community::RemovePerson(uint32_t personID)
{
	m_population.erase(
		std::remove_if(m_population.begin(), m_population.end(),
			[&](const Person& person) { return person.GetID() == personID; }), m_population.end()
	);
}

void DeseaseSpreadSimulation::Community::AddPlaces(Places places)
{
	m_places.Insert(places);
}

void DeseaseSpreadSimulation::Community::AddPopulation(std::vector<Person>& population)
{
	m_population.reserve(m_population.size() + population.size());
	m_population.insert(m_population.end(), population.begin(), population.end());
}

DeseaseSpreadSimulation::Person DeseaseSpreadSimulation::Community::TransferPerson(const Person& person)
{
	return TransferPerson(person.GetID());
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

	return &m_places.supplyStores.at(Random::RandomVectorIndex(m_places.supplyStores));
}

DeseaseSpreadSimulation::HardwareStore* DeseaseSpreadSimulation::Community::GetHardwareStore()
{
	if (m_places.hardwareStores.empty())
	{
		return nullptr;
	}

	return &m_places.hardwareStores.at(Random::RandomVectorIndex(m_places.hardwareStores));
}

DeseaseSpreadSimulation::Morgue* DeseaseSpreadSimulation::Community::GetMorgue()
{
	if (m_places.morgues.empty())
	{
		return nullptr;
	}
	
	return &m_places.morgues.at(Random::RandomVectorIndex(m_places.morgues));
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

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Community::TransferToPlace(Person* person, Place* place)
{
	person->GetWhereabouts()->RemovePerson(person);
	place->AddPerson(person);
	return place;
}
