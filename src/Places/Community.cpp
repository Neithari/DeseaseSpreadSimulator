#include "pch.h"
#include "Places/Community.h"
#include <gsl/gsl>

DeseaseSpreadSimulation::Community::Community(std::set<Person> population)
	:
	population(std::move(population))
{
}

void DeseaseSpreadSimulation::Community::AddPlace(std::unique_ptr<Place> place)
{
	places.push_back(std::move(place));
}

void DeseaseSpreadSimulation::Community::AddPerson(Person person)
{
	population.insert(std::move(person));
}

void DeseaseSpreadSimulation::Community::RemovePlace(uint32_t placeID)
{
	places.erase(
		std::remove_if(places.begin(), places.end(),
			[&](const std::unique_ptr<Place>& place) { return place->GetID() == placeID; }), places.end()
	);
}

void DeseaseSpreadSimulation::Community::RemovePerson(const Person& person)
{
	population.erase(person);
}

DeseaseSpreadSimulation::Person DeseaseSpreadSimulation::Community::TransferPerson(const Person& person)
{
	auto toTransfer = population.extract(person);

	if (toTransfer)
	{
		return std::move(toTransfer.value());
	}
	// This should never happen, because the person to transfer is calling it.
	// So we throw and silence compiler warnings
	throw(std::invalid_argument("person not found"));
	GSL_ASSUME(false);
}

const std::set<DeseaseSpreadSimulation::Person>& DeseaseSpreadSimulation::Community::GetPopulation() const
{
	return population;
}

const std::vector <std::unique_ptr<DeseaseSpreadSimulation::Place>>& DeseaseSpreadSimulation::Community::GetPlaces() const
{
	return places;
}
