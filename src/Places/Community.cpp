#include "pch.h"
#include "Places/Community.h"
#include <gsl/gsl>

void DeseaseSpreadSimulation::Community::AddPlace(std::unique_ptr<Place> place)
{
	places.push_back(std::move(place));
}

void DeseaseSpreadSimulation::Community::AddPerson(std::unique_ptr<Person> person)
{
	population.push_back(std::move(person));
}

void DeseaseSpreadSimulation::Community::RemovePlace(uint32_t placeID)
{
	places.erase(
		std::remove_if(places.begin(), places.end(),
			[&](const std::unique_ptr<Place>& place) { return place->GetID() == placeID; }), places.end()
	);
}

void DeseaseSpreadSimulation::Community::RemovePerson(uint32_t personID)
{
	population.erase(
		std::remove_if(population.begin(), population.end(),
			[&](const std::unique_ptr<Person>& person) { return person->GetID() == personID; }), population.end()
	);
}

std::unique_ptr<DeseaseSpreadSimulation::Person> DeseaseSpreadSimulation::Community::TransferPerson(uint32_t personID)
{
	auto toTransfer = std::find_if(population.begin(), population.end(),
		[&](const std::unique_ptr<Person>& person) { return person->GetID() == personID; });

	if (toTransfer != population.end())
	{
		auto transferPerson = std::move(*toTransfer);
		population.erase(toTransfer);
		return std::move(transferPerson);
	}
	// This should never happen, because the person to transfer is calling it.
	// So we throw and silence compiler warnings
	throw(std::invalid_argument("person not found"));
	GSL_ASSUME(false);
}

const std::vector<std::unique_ptr<DeseaseSpreadSimulation::Person>>& DeseaseSpreadSimulation::Community::GetPopulation() const
{
	return population;
}

const std::vector<std::unique_ptr<DeseaseSpreadSimulation::Place>>& DeseaseSpreadSimulation::Community::GetPlaces() const
{
	return places;
}
