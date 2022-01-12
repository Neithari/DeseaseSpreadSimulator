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
	// Add the place to the matching vector for easier lookup
	AddPlaceToLookupVector(place.get());

	// Append the new place to our vector of places
	m_places.push_back(std::move(place));
}

void DeseaseSpreadSimulation::Community::AddPlaces(std::vector<std::unique_ptr<Place>> places)
{
	// Add the places to the matching vectors for easier lookup
	for (auto& place: places)
	{
		AddPlaceToLookupVector(place.get());
	}
	// Append the new places to our vector of places
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

const std::vector<DeseaseSpreadSimulation::Home*>& DeseaseSpreadSimulation::Community::GetHomes() const
{
	return homes;
}

const DeseaseSpreadSimulation::Supply* DeseaseSpreadSimulation::Community::GetSupplyStore() const
{
	return supplyStores.at(RandomIndex(supplyStores.size()));
}

const DeseaseSpreadSimulation::HardwareStore* DeseaseSpreadSimulation::Community::GetHardwareStore() const
{
	return hardwareStores.at(RandomIndex(hardwareStores.size()));
}

const DeseaseSpreadSimulation::Morgue* DeseaseSpreadSimulation::Community::GetMorgue() const
{
	if (morgues.empty())
	{
		return nullptr;
	}

	return morgues.at(RandomIndex(morgues.size()));
}

void DeseaseSpreadSimulation::Community::AddPlaceToLookupVector(Place* place)
{
	// Add a pointer to home, supply, hardware stor and morgue to the vectors for easier lookup
	switch (place->GetType())
	{
	case Place_Type::Home:
		homes.push_back(static_cast<Home*>(place));
		break;
	case Place_Type::Supply:
		supplyStores.push_back(static_cast<Supply*>(place));
		break;
	case Place_Type::HardwareStore:
		hardwareStores.push_back(static_cast<HardwareStore*>(place));
		break;
	case Place_Type::Morgue:
		morgues.push_back(static_cast<Morgue*>(place));
		break;
	case Place_Type::Workplace:
		break;
	case Place_Type::School:
		break;
	default:
		break;
	}
}
