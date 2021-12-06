#include "pch.h"
#include "Places/Community.h"
#include <gsl/gsl>

DeseaseSpreadSimulation::Community::Community(std::vector<std::unique_ptr<Person>> population, std::vector<std::unique_ptr<Place>> places)
	:
	m_population(std::move(population)),
	m_places(std::move(places))
{
}

void DeseaseSpreadSimulation::Community::AddPlace(std::unique_ptr<Place> place)
{
	m_places.push_back(std::move(place));
}

void DeseaseSpreadSimulation::Community::AddPlaces(std::vector<std::unique_ptr<Place>> places)
{
	TransferUniquePtrVector<Place>(std::move(places), m_places);
}

void DeseaseSpreadSimulation::Community::AddPerson(std::unique_ptr<Person> person)
{
	m_population.push_back(std::move(person));
}

void DeseaseSpreadSimulation::Community::AddPopulation(std::vector<std::unique_ptr<Person>> population)
{
	TransferUniquePtrVector<Person>(std::move(population), m_population);
}

void DeseaseSpreadSimulation::Community::RemovePlace(uint32_t placeID)
{
	m_places.erase(
		std::remove_if(m_places.begin(), m_places.end(),
			[&](const std::unique_ptr<Place>& place) { return place->GetID() == placeID; }), m_places.end()
	);
}

void DeseaseSpreadSimulation::Community::RemovePerson(uint32_t personID)
{
	m_population.erase(
		std::remove_if(m_population.begin(), m_population.end(),
			[&](const std::unique_ptr<Person>& person) { return person->GetID() == personID; }), m_population.end()
	);
}

std::unique_ptr<DeseaseSpreadSimulation::Person> DeseaseSpreadSimulation::Community::TransferPerson(uint32_t personID)
{
	auto toTransfer = std::find_if(m_population.begin(), m_population.end(),
		[&](const std::unique_ptr<Person>& person) { return person->GetID() == personID; });
	
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

std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>>& DeseaseSpreadSimulation::Community::GetPopulation()
{
	return m_population;
}

std::vector<std::unique_ptr<DeseaseSpreadSimulation::Place>>& DeseaseSpreadSimulation::Community::GetPlaces()
{
	return m_places;
}
