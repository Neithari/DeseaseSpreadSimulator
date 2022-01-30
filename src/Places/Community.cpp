#include "pch.h"
#include "Places/Community.h"
#include <gsl/gsl>

DeseaseSpreadSimulation::Community::Community(std::vector<Person> population, Places places)
	:
	m_population(std::move(population)),
	m_places(std::move(places))
{
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

void DeseaseSpreadSimulation::Community::RemovePlace(const Home& home)
{
	m_places.homes.erase(
		std::remove_if(m_places.homes.begin(), m_places.homes.end(),
			[&](const Home& place) { return place.GetID() == home.GetID(); }), m_places.homes.end()
	);
}

void DeseaseSpreadSimulation::Community::RemovePlace(const Supply& store)
{
	m_places.supplyStores.erase(
		std::remove_if(m_places.supplyStores.begin(), m_places.supplyStores.end(),
			[&](const Supply& place) { return place.GetID() == store.GetID(); }), m_places.supplyStores.end()
	);
}

void DeseaseSpreadSimulation::Community::RemovePlace(const Workplace& workplace)
{
	m_places.workplaces.erase(
		std::remove_if(m_places.workplaces.begin(), m_places.workplaces.end(),
			[&](const Workplace& place) { return place.GetID() == workplace.GetID(); }), m_places.workplaces.end()
	);
}

void DeseaseSpreadSimulation::Community::RemovePlace(const School& school)
{
	m_places.schools.erase(
		std::remove_if(m_places.schools.begin(), m_places.schools.end(),
			[&](const School& place) { return place.GetID() == school.GetID(); }), m_places.schools.end()
	);
}

void DeseaseSpreadSimulation::Community::RemovePlace(const HardwareStore& store)
{
	m_places.hardwareStores.erase(
		std::remove_if(m_places.hardwareStores.begin(), m_places.hardwareStores.end(),
			[&](const HardwareStore& place) { return place.GetID() == store.GetID(); }), m_places.hardwareStores.end()
	);
}

void DeseaseSpreadSimulation::Community::RemovePlace(const Morgue& morgue)
{
	m_places.morgues.erase(
		std::remove_if(m_places.morgues.begin(), m_places.morgues.end(),
			[&](const Morgue& place) { return place.GetID() == morgue.GetID(); }), m_places.morgues.end()
	);
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

void DeseaseSpreadSimulation::Community::AddPopulation(std::vector<Person> population)
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
		return std::move(transferPerson);
	}
	// This should never happen, because the person to transfer is calling it.
	// So we throw and silence compiler warnings
	throw(std::invalid_argument("person not found"));
	GSL_ASSUME(false);
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
