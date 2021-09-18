#include "pch.h"
#include "Places/CommunityBuilder.h"
#include "Places/Community.h"
#include "Person/PersonPopulator.h"

DeseaseSpreadSimulation::Community DeseaseSpreadSimulation::CommunityBuilder::CreateCommunity(size_t populationSize, Country country) const
{
	Community community;

	CreatePlaces(populationSize, country, community);
	CreatePopulation(populationSize, country, community);

	return std::move(community);
}

void DeseaseSpreadSimulation::CommunityBuilder::CreatePlaces(size_t populationSize, Country country, Community& outCommunity) const
{
	// Sum all home members and create enough homes
	size_t oneMemberHomes = static_cast<size_t>(populationSize / PersonPopulator::householdUSA.oneMember);
	size_t twoToThreeMemberHomes = static_cast<size_t>(populationSize / PersonPopulator::householdUSA.twoToThreeMembers);
	size_t fourToFiveMemberHomes = static_cast<size_t>(populationSize / PersonPopulator::householdUSA.fourToFiveMembers);
	size_t sixPlusMemberHomes = static_cast<size_t>(populationSize / PersonPopulator::householdUSA.sixPlusMembers);
	size_t sum = oneMemberHomes + twoToThreeMemberHomes + fourToFiveMemberHomes + sixPlusMemberHomes;
	for (size_t i = 0; i < sum; i++)
	{
		outCommunity.AddPlace(std::make_unique<Home>());
	}


	// Create workplaces for people between 20 and 69
	auto countryDistribution = std::move(PersonPopulator::GetCountryDistribution(country));
	size_t workingPeople = 0;
	// First get number of working people
	for (const auto& distribution : countryDistribution)
	{
		if (distribution.ageGroup > Age_Group::UnderTwenty &&  distribution.ageGroup <= Age_Group::UnderSeventy)
		{
			workingPeople += static_cast<size_t>(populationSize / distribution.percent);
		}
	}
	// Workplace size estimates https://www.statista.com/statistics/944669/current-office-size-full-time-employees-usa/
	std::array<float, 5> workplaceSize{ 0.2648f, 0.308f, 0.1908f, 0.0821f, 0.1542f };
	float workplaceCount = 0.f;

	// Then get the workplace counts for the size groups and sum them
	for (size_t i = 0; i < workplaceSize.size(); i++)
	{
		workplaceCount = (workingPeople * workplaceSize.at(i)) / (25 + 50 * i);
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
		supplyCount++;
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

	while (true)
	{
		auto person = personCreator.GetNewPerson(AssignHome());

		// Break the loop when we got the whole population
		if (!person)
		{
			break;
		}

		// Assigne a workplace when the person is in working age
		if (person->GetAgeGroup() > Age_Group::UnderTwenty && person->GetAgeGroup() <= Age_Group::UnderSeventy)
		{
			for (const auto& place : outCommunity.GetPlaces())
			{
				// TODO: implement sorting for workplace and assign a workplace to the person
				// to silence compiler warning
				if (place->GetID() < 0) {}
			}
		}
		outCommunity.AddPerson(std::move(person));
	}
}

DeseaseSpreadSimulation::Home* DeseaseSpreadSimulation::CommunityBuilder::AssignHome() const
{
	return nullptr;
}
