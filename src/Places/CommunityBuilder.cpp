#include "pch.h"
#include "Places/CommunityBuilder.h"
#include "Places/Community.h"
#include "Person/PersonPopulator.h"

DeseaseSpreadSimulation::Community DeseaseSpreadSimulation::CommunityBuilder::CreateCommunity(const size_t populationSize, const Country country) const
{
	Community community;

	CreatePlaces(populationSize, country, community);
	CreatePopulation(populationSize, country, community);

	return community;
}

void DeseaseSpreadSimulation::CommunityBuilder::CreatePlaces(const size_t populationSize, const Country country, Community& outCommunity) const
{
	// Get the person count per household category and create the correct number of homes
	// Sum the home counts to create the needed number of homes
	size_t sum = 0;
	for (const auto& count : GetHomeCounts(populationSize, country))
	{
		sum += count;
	}
	for (size_t i = 0; i < sum; i++)
	{
		outCommunity.AddPlace(std::make_unique<Home>());
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
		outCommunity.AddPlace(std::make_unique<Workplace>());
	}

	// Create one supply building and a morgue for every 5000 persons at least one of each
	size_t supplyCount = populationSize / 5000;
	if (supplyCount < 1)
	{
		supplyCount = 1;
	}
	for (size_t i = 0; i < supplyCount; i++)
	{
		outCommunity.AddPlace(std::make_unique<Supply>());
		outCommunity.AddPlace(std::make_unique<HardwareStore>());
		outCommunity.AddPlace(std::make_unique<Morgue>());
	}
}

void DeseaseSpreadSimulation::CommunityBuilder::CreatePopulation(const size_t populationSize, const Country country, Community& outCommunity) const
{
	PersonPopulator personCreator(populationSize, PersonPopulator::GetCountryDistribution(country));

	// Create a vector with all homes and a vector with all workplaces to assigne them later
	std::vector<Place*> homes;
	std::vector<Place*> workplaces;

	for (const auto& place : outCommunity.GetPlaces())
	{
		switch (place->GetType())
		{
		case Place::Place_Type::Home:
			homes.push_back(place.get());
			break;
		case Place::Place_Type::Workplace:
			workplaces.push_back(place.get());
			break;
		case Place::Place_Type::Supply:
			break;
		case Place::Place_Type::HardwareStore:
			break;
		case Place::Place_Type::Morgue:
			break;
		default:
			break;
		}
	}

	// Create an array containing all workplaces sorted by size
	auto workplacesBySize(WorkplacesBySize(populationSize, country, std::move(workplaces)));
	
	// Create the population
	while (true)
	{
		auto person = personCreator.GetNewPerson(AssignHome(populationSize, country));

		// Break the loop when we got the whole population
		if (!person)
		{
			break;
		}

		// Assigne a workplace when the person is in working age and there is a workplace
		bool noWorkplace = true;
		for (const auto& work : workplacesBySize)
		{
			if (work.size() > 0)
			{
				noWorkplace = false;
				break;
			}
		}
		if (!noWorkplace && person->GetAgeGroup() > Age_Group::UnderTwenty && person->GetAgeGroup() <= Age_Group::UnderSeventy)
		{
			// First choose a weighted workplace vector, then assigne uniform under the same size workplaces
			std::random_device seed;
			std::mt19937 generator(seed());
			// Create the distribution with the workplaceSize array as weights
			std::discrete_distribution<size_t> distribution(workplaceSize.begin(), workplaceSize.end());
			size_t distIndex = distribution(generator);
			// Get a new index until the vector is not empty
			while (workplacesBySize.at(distIndex).empty())
			{
				distIndex = distribution(generator);
			}
			std::uniform_int_distribution<size_t> uniform(0, (workplacesBySize.at(distIndex).size() - 1));
			person->SetWorkplace(workplacesBySize.at(distIndex).at(uniform(generator)));
		}
		// Add the created person to the community
		outCommunity.AddPerson(std::move(person));
	}
}

DeseaseSpreadSimulation::Home* DeseaseSpreadSimulation::CommunityBuilder::AssignHome(const size_t populationSize, const Country country) const
{
	std::random_device seed;
	std::mt19937 generator(seed());
	std::discrete_distribution<size_t> distribution({	PersonPopulator::GetHouseholdDistribution(country).oneMember,
														PersonPopulator::GetHouseholdDistribution(country).twoToThreeMembers,
														PersonPopulator::GetHouseholdDistribution(country).fourToFiveMembers, 
														PersonPopulator::GetHouseholdDistribution(country).sixPlusMembers
													});
	// to silence warning
	if (populationSize)
	{

	}
	return nullptr;
}

size_t DeseaseSpreadSimulation::CommunityBuilder::WorkingPeopleNumber(const size_t populationSize, const Country country) const
{
	// TODO: Need a better way to get the working people. Not in sync with PersonPopulator::GetNewPerson()
	auto countryDistribution = std::move(PersonPopulator::GetCountryDistribution(country));
	size_t workingPeople = 0;
	for (const auto& distribution : countryDistribution)
	{
		if (distribution.ageGroup > Age_Group::UnderTwenty && distribution.ageGroup <= Age_Group::UnderSeventy)
		{
			workingPeople += llround(populationSize * (double)distribution.percent);
		}
	}
	return workingPeople;
}

std::array<size_t, 4> DeseaseSpreadSimulation::CommunityBuilder::GetHomeCounts(const size_t populationSize, const Country country) const
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

std::array<std::vector<DeseaseSpreadSimulation::Place*>, 4> DeseaseSpreadSimulation::CommunityBuilder::HomesByMemberCount(const size_t populationSize, const Country country, std::vector<Place*> homes) const
{
	auto homeCounts(GetHomeCounts(populationSize, country));

	auto from = homes.begin();
	auto to = homes.begin() + homeCounts.at(0);
	std::vector<Place*> oneMember(from, to);
	from = to;
	to += homeCounts.at(1);
	std::vector<Place*> twoToThreeMembers(from, to);
	from = to;
	to += homeCounts.at(2);
	std::vector<Place*> fourToFiveMembers(from, to);
	from = to;
	std::vector<Place*> sixPlusMembers(from, homes.end());

	return { oneMember, twoToThreeMembers, fourToFiveMembers, sixPlusMembers };
}

std::array<std::vector<DeseaseSpreadSimulation::Place*>, 5> DeseaseSpreadSimulation::CommunityBuilder::WorkplacesBySize(const size_t populationSize, const Country country, std::vector<Place*> workplaces) const
{
	// First get number of working people
	size_t workingPeople = WorkingPeopleNumber(populationSize, country);

	// Then get the workplace counts for the size groups and transfer the right amount into a vector inside the bySize array
	std::array<std::vector<Place*>, 5> workplacesBySize;
	for (size_t i = 0; i < workplaceSize.size(); i++)
	{
		for (size_t j = 0; j < static_cast<size_t>((workingPeople * workplaceSize.at(i)) / (25 + 50 * i)); j++)
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
