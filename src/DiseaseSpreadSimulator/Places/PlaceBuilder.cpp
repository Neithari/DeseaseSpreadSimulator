#include "Places/PlaceBuilder.h"
#include <cmath>
#include <numeric>
#include "Enums.h"
#include "Statistics.h"
#include "Person/PersonPopulator.h"

DiseaseSpreadSimulation::Places DiseaseSpreadSimulation::PlaceBuilder::CreatePlaces(const size_t populationSize, const Country country)
{
	Places places;
	// Return early with no locations when the population size is 0
	if (populationSize == 0)
	{
		return places;
	}

	// Get the person count per household category and create the correct number of homes
	// Sum the home counts to create the needed number of homes
	auto homeCounts = GetHomeCounts(static_cast<float>(populationSize), country);
	size_t sum = std::accumulate(homeCounts.begin(), homeCounts.end(), 0ULL);

	places.homes.reserve(sum);
	for (size_t i = 0; i < sum; i++)
	{
		places.homes.emplace_back();
	}

	// Create workplaces for people between 20 and 69
	// Get the workplace counts for the size groups and sum them
	float workplaceCount = 0.f;
	for (size_t i = 0; i < Statistics::workplaceSize.size(); i++)
	{
		// Workplaces starting with 25 employees and increasing by 50 per size
		workplaceCount += (PersonPopulator::WorkingPeopleCountFloat(populationSize, country) * Statistics::workplaceSize.at(i)) / static_cast<float>(25 + 50 * i);
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

	// Create schools
	auto schoolKidsCount = PersonPopulator::SchoolKidsCount(populationSize, country);
	auto schoolSize = Statistics::AverageSchoolSize(country);
	size_t schoolCount;
	// Open one extra school if there are 100 kids more than the average
	size_t rest = schoolKidsCount % schoolSize;
	if (schoolKidsCount < 100)
	{
		schoolCount = 1;
	}
	else if (rest < 100)
	{
		schoolCount = schoolKidsCount / schoolSize;
	}
	else
	{
		schoolCount = static_cast<size_t>(std::ceil(static_cast<double>(schoolKidsCount) / static_cast<double>(schoolSize)));
	}
	for (size_t i = 0; i < schoolCount; i++)
	{
		places.schools.emplace_back();
	}

	return places;
}

std::array<std::vector<DiseaseSpreadSimulation::Workplace*>, 5> DiseaseSpreadSimulation::PlaceBuilder::WorkplacesBySize(const size_t populationSize, const Country country, std::vector<Workplace*> workplaces)
{
	// Get the workplace counts for the size groups and transfer the right amount into a vector inside the bySize array
	std::array<std::vector<Workplace*>, 5> workplacesBySize;
	for (size_t i = 0; i < Statistics::workplaceSize.size(); i++)
	{
		for (size_t j = 0; j < static_cast<size_t>((PersonPopulator::WorkingPeopleCountFloat(populationSize, country) * Statistics::workplaceSize.at(i)) / static_cast<float>(25 + 50 * i)); j++)
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

std::array<size_t, 4> DiseaseSpreadSimulation::PlaceBuilder::GetHomeCounts(const float populationSize, const Country country)
{
	// The home count is equal to the person count living in such a home devided by the median person count in the category
	// To get the person count we multiply the population size with the percentage of the distribution
	// Values are rounded at the end and cast to size_t
	return {
		static_cast<size_t>(std::llround(populationSize * PersonPopulator::GetHouseholdDistribution(country).oneMember)),               // One member homes
		static_cast<size_t>(std::roundf(populationSize * PersonPopulator::GetHouseholdDistribution(country).twoToThreeMembers) / 2.5F), // Two to three member homes
		static_cast<size_t>(std::roundf(populationSize * PersonPopulator::GetHouseholdDistribution(country).fourToFiveMembers) / 4.5F), // Four to five member homes
		static_cast<size_t>(std::roundf(populationSize * PersonPopulator::GetHouseholdDistribution(country).sixPlusMembers) / 6.5F)     // Six and more member homes
	};
}
