#include "pch.h"
#include "Places/CommunityBuilder.h"
#include "Places/Community.h"
#include "Person/PersonPopulator.h"

DeseaseSpreadSimulation::Community DeseaseSpreadSimulation::CommunityBuilder::CreateCommunity(size_t populationSize, Country country) const
{
	Community community;

	CreatePlaces(populationSize, country, community);
	CreatePopulation(populationSize, country, community);

	return community;
}

void DeseaseSpreadSimulation::CommunityBuilder::CreatePlaces(size_t populationSize, Country country, Community& outCommunity) const
{
	// Get the person count per household category and create the correct number of homes
	for (size_t i = 0; i < GetHomeCount(populationSize, country); i++)
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

void DeseaseSpreadSimulation::CommunityBuilder::CreatePopulation(size_t populationSize, Country country, Community& outCommunity) const
{
	PersonPopulator personCreator(populationSize, PersonPopulator::GetCountryDistribution(country));

	// Create a vector with all workplaces to assigne them later
	std::vector<Place*> workplaces;

	for (const auto& place : outCommunity.GetPlaces())
	{
		// TODO: implement sorting for workplace and assign a workplace to the person
		if (place->GetType() == Place::Place_Type::Workplace)
		{
			workplaces.push_back(place.get());
		}
	}

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

	// Create the population
	while (true)
	{
		auto person = personCreator.GetNewPerson(AssignHome());

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
			std::discrete_distribution<size_t> distribution({ 2649, 3080, 1908, 821, 1542 });
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

DeseaseSpreadSimulation::Home* DeseaseSpreadSimulation::CommunityBuilder::AssignHome() const
{
	// TODO: Implement a home assigne function
	return nullptr;
}

size_t DeseaseSpreadSimulation::CommunityBuilder::WorkingPeopleNumber(size_t populationSize, Country country) const
{
	// TODO: Need a better way to get the working people. Not in sync with PersonPopulator::GetNewPerson()
	auto countryDistribution = std::move(PersonPopulator::GetCountryDistribution(country));
	size_t workingPeople = 0;
	for (const auto& distribution : countryDistribution)
	{
		if (distribution.ageGroup > Age_Group::UnderTwenty && distribution.ageGroup <= Age_Group::UnderSeventy)
		{
			workingPeople += static_cast<size_t>(populationSize * distribution.percent);
		}
	}
	return workingPeople;
}

size_t DeseaseSpreadSimulation::CommunityBuilder::GetHomeCount(size_t populationSize, Country country) const
{
	// Get the person count per household category
	double oneMemberHomesPersonCount = populationSize * PersonPopulator::GetHouseholdDistribution(country).oneMember;
	double twoToThreeMemberHomesPersonCount = populationSize * PersonPopulator::GetHouseholdDistribution(country).twoToThreeMembers;
	double fourToFiveMemberHomesPersonCount = populationSize * PersonPopulator::GetHouseholdDistribution(country).fourToFiveMembers;
	double sixPlusMemberHomesPersonCount = populationSize * PersonPopulator::GetHouseholdDistribution(country).sixPlusMembers;

	// The home count is equal to the person count living in such a home devided by the median person count in the category
	size_t oneMemberHomes = static_cast<size_t>(oneMemberHomesPersonCount);
	size_t twoToThreeMemberHomes = static_cast<size_t>(twoToThreeMemberHomesPersonCount / 2.5);
	size_t fourToFiveMemberHomes = static_cast<size_t>(fourToFiveMemberHomesPersonCount / 4.5);
	size_t sixPlusMemberHomes = static_cast<size_t>(sixPlusMemberHomesPersonCount / 6.5);

	// Return the sum
	return oneMemberHomes + twoToThreeMemberHomes + fourToFiveMemberHomes + sixPlusMemberHomes;
}
