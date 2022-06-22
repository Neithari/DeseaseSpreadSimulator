#include "Places/PlaceBuilder.h"
#include <cmath>
#include <numeric>
#include "Enums.h"
#include "Statistics.h"
#include "Person/PersonPopulator.h"


namespace DiseaseSpreadSimulation::PlaceBuilder
{
	// Workplaces starting with 25 employees and increasing by 50 per size
	constexpr float employeesStart{25.F};
	constexpr float sizeInrease{50.F};

Places CreatePlaces(const size_t populationSize, const Country country)
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
	float workplaceCount{0.F};
	for (size_t i = 0; i < Statistics::workplaceSizePercent.size(); i++)
	{
		// Workplaces starting with 25 employees and increasing by 50 per size
		float employeesPerWorkplace{employeesStart + sizeInrease * static_cast<float>(i)};
		// Get the amount of persons per size category and divide it by the employee count
		workplaceCount += (PersonPopulator::WorkingPeopleCountFloat(populationSize, country) * Statistics::workplaceSizePercent.at(i)) / employeesPerWorkplace;
	}

	// Add the calculated amount of workplaces
	for (size_t i = 0; i < static_cast<size_t>(workplaceCount); i++)
	{
		places.workplaces.emplace_back();
	}

	// Create one supply building and a morgue for every 5000 persons. At least one of each
	static constexpr auto personThreshold{5000U};
	size_t supplyCount{populationSize / personThreshold};
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
	size_t schoolCount{0};
	// Open one extra school if there are 100 kids more than the average
	static constexpr auto extraSchoolThreshold{100};
	size_t rest = schoolKidsCount % schoolSize;
	if (schoolKidsCount < extraSchoolThreshold)
	{
		schoolCount = 1;
	}
	else if (rest < extraSchoolThreshold)
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

std::array<std::vector<Workplace*>, 5> WorkplacesBySize(const size_t populationSize, const Country country, std::vector<Workplace*> workplaces)
{
	// Get the workplace counts for the size groups and transfer the right amount into a vector inside the bySize array
	std::array<std::vector<Workplace*>, Statistics::workplaceSizePercent.size()> workplacesBySize;
	for (size_t i = 0; i < Statistics::workplaceSizePercent.size(); i++)
	{
		// Workplaces starting with 25 employees and increasing by 50 per size
		float employeesPerWorkplace{employeesStart + sizeInrease * static_cast<float>(i)};
		for (size_t j = 0; j < static_cast<size_t>((PersonPopulator::WorkingPeopleCountFloat(populationSize, country) * Statistics::workplaceSizePercent.at(i)) / employeesPerWorkplace); j++)
		{
			workplacesBySize.at(i).push_back(workplaces.back());
			workplaces.pop_back();
		}
	}
	// Assigne leftovers to under 20 people workplaces
	while (!workplaces.empty())
	{
		workplacesBySize.at(0).push_back(workplaces.back());
		workplaces.pop_back();
	}
	return workplacesBySize;
}

std::array<size_t, 4> GetHomeCounts(const float populationSize, const Country country)
{
	// The home count is equal to the person count living in such a home devided by the median person count in the category
	// To get the person count we multiply the population size with the percentage of the distribution
	// Values are rounded at the end and cast to size_t
	constexpr auto medianTwoThreeMember{2.5F};
	constexpr auto medianFourFiveMember{4.5F};
	constexpr auto medianSixSevenMember{6.5F};
	return {
		static_cast<size_t>(std::llround(populationSize * PersonPopulator::GetHouseholdDistribution(country).oneMember)),							   // One member homes
		static_cast<size_t>(std::roundf(populationSize * PersonPopulator::GetHouseholdDistribution(country).twoToThreeMembers) / medianTwoThreeMember), // Two to three member homes
		static_cast<size_t>(std::roundf(populationSize * PersonPopulator::GetHouseholdDistribution(country).fourToFiveMembers) / medianFourFiveMember), // Four to five member homes
		static_cast<size_t>(std::roundf(populationSize * PersonPopulator::GetHouseholdDistribution(country).sixPlusMembers) / medianSixSevenMember)     // Six and more member homes
	};
}

} // namespace DiseaseSpreadSimulation::PlaceBuilder
