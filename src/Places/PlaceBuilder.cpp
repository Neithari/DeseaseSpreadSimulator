#include "pch.h"
#include "Places/PlaceBuilder.h"
#include "Person/PersonPopulator.h"

DeseaseSpreadSimulation::Places DeseaseSpreadSimulation::PlaceBuilder::CreatePlaces(const size_t populationSize, const Country country) const
{
	Places places;
	// Get the person count per household category and create the correct number of homes
	// Sum the home counts to create the needed number of homes
	size_t sum = 0;
	for (const auto& count : GetHomeCounts(populationSize, country))
	{
		sum += count;
	}
	for (size_t i = 0; i < sum; i++)
	{
		places.homes.emplace_back();
	}

	// Create workplaces for people between 20 and 69
	// First get number of working people
	size_t workingPeople = PersonPopulator::WorkingPeopleCount(populationSize, country);

	// Then get the workplace counts for the size groups and sum them
	float workplaceCount = 0.f;
	for (size_t i = 0; i < Statistics::workplaceSize.size(); i++)
	{
		workplaceCount += (workingPeople * Statistics::workplaceSize.at(i)) / (25 + 50 * i);
	}

	// Add the calculated amount of workplaces
	for (size_t i = 0; i < static_cast<size_t>(workplaceCount); i++)
	{
		places.workplaces.emplace_back();
	}

	// Create one supply building and a morgue for every 5000 persons. At least one of each
	size_t supplyCount = populationSize / 5000;
	if (supplyCount < 1)
	{
		supplyCount = 1;
	}
	for (size_t i = 0; i < supplyCount; i++)
	{
		places.supplyStores.emplace_back();
		places.hardwareStores.emplace_back();
		places.morgues.emplace_back();
	}

	return places;
}

std::array<std::vector<DeseaseSpreadSimulation::Workplace*>, 5> DeseaseSpreadSimulation::PlaceBuilder::WorkplacesBySize(const size_t populationSize, const Country country, std::vector<Workplace*> workplaces)
{
	// First get number of working people
	size_t workingPeople = PersonPopulator::WorkingPeopleCount(populationSize, country);

	// Then get the workplace counts for the size groups and transfer the right amount into a vector inside the bySize array
	std::array<std::vector<Workplace*>, 5> workplacesBySize;
	for (size_t i = 0; i < Statistics::workplaceSize.size(); i++)
	{
		for (size_t j = 0; j < static_cast<size_t>((workingPeople * Statistics::workplaceSize.at(i)) / (25 + 50 * i)); j++)
		{
			workplacesBySize.at(i).push_back(workplaces.back());
			workplaces.pop_back();
		}
	}
	// Assigne leftovers to under 20 people workplaces
	while (workplaces.size() > 0)
	{
		workplacesBySize.at(0).push_back(workplaces.back());
		workplaces.pop_back();
	}
	return workplacesBySize;
}

std::array<size_t, 4> DeseaseSpreadSimulation::PlaceBuilder::GetHomeCounts(const size_t populationSize, const Country country)
{
	// The home count is equal to the person count living in such a home devided by the median person count in the category
	// To get the person count we multiply the population size with the percentage of the distribution
	// Values are rounded at the end and cast to size_t
	return { static_cast<size_t>(llround(populationSize * PersonPopulator::GetHouseholdDistribution(country).oneMember)),				// One member homes
			 static_cast<size_t>(llround(populationSize * PersonPopulator::GetHouseholdDistribution(country).twoToThreeMembers) / 2.5),	// Two to three member homes
			 static_cast<size_t>(llround(populationSize * PersonPopulator::GetHouseholdDistribution(country).fourToFiveMembers) / 4.5),	// Four to five member homes
			 static_cast<size_t>(llround(populationSize * PersonPopulator::GetHouseholdDistribution(country).sixPlusMembers) / 6.5)		// Six and more member homes
	};
}
