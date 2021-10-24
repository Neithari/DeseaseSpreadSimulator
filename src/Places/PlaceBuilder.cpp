#include "pch.h"
#include "Places/PlaceBuilder.h"
#include "Person/PersonPopulator.h"

std::vector<std::unique_ptr<DeseaseSpreadSimulation::Place>> DeseaseSpreadSimulation::PlaceBuilder::CreatePlaces(const size_t populationSize, const Country country) const
{
	std::vector<std::unique_ptr<Place>> places;
	// Get the person count per household category and create the correct number of homes
	// Sum the home counts to create the needed number of homes
	size_t sum = 0;
	for (const auto& count : GetHomeCounts(populationSize, country))
	{
		sum += count;
	}
	for (size_t i = 0; i < sum; i++)
	{
		places.push_back(std::make_unique<Home>());
	}

	// Create workplaces for people between 20 and 69
	// First get number of working people
	size_t workingPeople = WorkingPeopleNumber(populationSize, country);
	float workplaceCount = 0.f;

	// Then get the workplace counts for the size groups and sum them
	for (size_t i = 0; i < workplaceSize.size(); i++)
	{
		workplaceCount += (workingPeople * workplaceSize.at(i)) / (25 + 50 * i);
	}

	// Add the calculated amount of workplaces to the community
	for (size_t i = 0; i < static_cast<size_t>(workplaceCount); i++)
	{
		places.emplace_back(std::make_unique<Workplace>());
	}

	// Create one supply building and a morgue for every 5000 persons at least one of each
	size_t supplyCount = populationSize / 5000;
	if (supplyCount < 1)
	{
		supplyCount = 1;
	}
	for (size_t i = 0; i < supplyCount; i++)
	{
		places.push_back(std::make_unique<Supply>());
		places.push_back(std::make_unique<HardwareStore>());
		places.push_back(std::make_unique<Morgue>());
	}

	return places;
}

size_t DeseaseSpreadSimulation::PlaceBuilder::WorkingPeopleNumber(const size_t populationSize, const Country country) const
{
	// TODO: Need a better way to get the working people. Not in sync with PersonPopulator::GetNewPerson()
	auto countryDistribution = std::move(PersonPopulator::GetCountryDistribution(country));
	size_t workingPeople = 0;
	// For every human distribution in country distribution...
	for (const auto& humanDistribution : countryDistribution)
	{
		// ...check if the distribution is inside working age...
		if (humanDistribution.ageGroup > Age_Group::UnderTwenty && humanDistribution.ageGroup <= Age_Group::UnderSeventy)
		{
			// ...and if it is, sum the rounded population size with the distribution applied
			workingPeople += llround(populationSize * static_cast<double>(humanDistribution.percent));
		}
	}
	return workingPeople;
}

std::array<size_t, 4> DeseaseSpreadSimulation::PlaceBuilder::GetHomeCounts(const size_t populationSize, const Country country) const
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
