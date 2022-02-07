#include "pch.h"
#include "Person/PersonPopulator.h"
#include "Person/PersonBehavior.h"
#include "Places/PlaceBuilder.h"

DeseaseSpreadSimulation::PersonPopulator::PersonPopulator(size_t populationSize, std::vector<Statistics::HumanDistribution> humanDistribution)
	:
	m_populationSize(populationSize),
	leftover(populationSize),
	ageDistribution(std::move(humanDistribution)),
	currentHumanDistribution(ageDistribution.front())
{
	// Set the currentHumanCount to a percent of the population and to 1 if this will return 0
	currentHumanCount = DistributionToCountHelper(populationSize, currentHumanDistribution.percent);
	if (currentHumanCount == 0)
	{
		currentHumanCount = 1;
	}
}

std::vector<DeseaseSpreadSimulation::Person> DeseaseSpreadSimulation::PersonPopulator::CreatePopulation(size_t populationSize, Country country, std::vector<Home>& homes, std::vector<Workplace>& workplaces)
{
	std::vector<Person> population;

	// This bool will ensure that a workplace is only assigned if there are workplaces
	bool noWorkplace = workplaces.empty();

	// Create an array containing all workplaces sorted by size
	std::vector<Workplace*> sortedWorkplaces;
	for (auto& workplace : workplaces)
	{
		sortedWorkplaces.push_back(&workplace);
	}
	auto workplacesBySize(PlaceBuilder::WorkplacesBySize(populationSize, country, std::move(sortedWorkplaces)));

	// Create the population
	while (!allAssigned)
	{
		// Get a new person
		auto person = GetNewPerson();

		// Assigne a workplace when the person is in working age and there are workplaces
		if (!noWorkplace && person.GetAgeGroup() > Age_Group::UnderTwenty && person.GetAgeGroup() <= Age_Group::UnderSeventy)
		{
			person.SetWorkplace(AssignWorkplace(workplacesBySize));
		}
		// Add the created person to the community
		population.push_back(std::move(person));
	}

	AssigneHomesToPopulation(population, homes, country);

	return population;
}

DeseaseSpreadSimulation::Person DeseaseSpreadSimulation::PersonPopulator::GetNewPerson(Community* community, Home* home)
{
	// As long as we don't have assigned the full population create a new person with age and sex according to our distribution
	// When the currentHumanCount is 0...
	if (currentHumanCount == 0)
	{
		// ...we set the new current distribution...
		currentHumanDistribution = ageDistribution.at(ageDistributionIndex);
		
		if (!lastFew)
		{
			// ...set the currentHumanCount to a percent of the population...
			currentHumanCount = DistributionToCountHelper(m_populationSize, currentHumanDistribution.percent);
		}
		// ...and to 1 if this will return 0...
		if (currentHumanCount == 0)
		{
			currentHumanCount = 1;
		}
		
		// ...advance the index and check if we used all distributions...
		if (++ageDistributionIndex >= ageDistribution.size())
		{
			// ...if yes we start again at the beginning...
			ageDistributionIndex = 0;
			// ...and distribute 1 to every human distribution until no leftover
			lastFew = true;
		}
	}

	leftover--;
	currentHumanCount--;
	if (leftover <= 0)
	{
		allAssigned = true;
	}

	return Person(currentHumanDistribution.ageGroup, currentHumanDistribution.sex, PersonBehavior(), community, home);
}

size_t DeseaseSpreadSimulation::PersonPopulator::WorkingPeopleCount(const size_t populationSize, const Country country)
{
	// TODO: Need a better way to get the working people. Not in sync with PersonPopulator::GetNewPerson()
	auto countryDistribution = std::move(GetCountryDistribution(country));
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

std::array<std::vector<DeseaseSpreadSimulation::Home*>, 4> DeseaseSpreadSimulation::PersonPopulator::HomesByMemberCount(const size_t populationSize, const Country country, std::vector<Home*> homes)
{
	auto homeCounts(PlaceBuilder::GetHomeCounts(populationSize, country));

	// Set iterators to copy a part of the homes vector into the new vectores separated by size
	auto from = homes.cbegin();
	auto to = homes.cbegin() + homeCounts.at(0);
	std::vector<Home*> oneMember(from, to);
	// Change the iterators for the next size and do the same for every size
	from = to;
	to += homeCounts.at(1);
	std::vector<Home*> twoToThreeMembers(from, to);
	from = to;
	to += homeCounts.at(2);
	std::vector<Home*> fourToFiveMembers(from, to);
	from = to;
	std::vector<Home*> sixPlusMembers(from, homes.cend());

	// Return an array with all size vectors
	return { oneMember, twoToThreeMembers, fourToFiveMembers, sixPlusMembers };
}

size_t DeseaseSpreadSimulation::PersonPopulator::DistributionToCountHelper(size_t count, float percent)
{
	// Scale count by percent and then omit the decimal
	return static_cast<size_t>(count * static_cast<double>(percent));
}

DeseaseSpreadSimulation::Home* DeseaseSpreadSimulation::PersonPopulator::AssignHome(const Country country, const Age_Group ageGroup, const std::array<std::vector<Home*>, 4>& homesByMemberCount)
{
	// Create the distribution
	std::array<double, 4> distributionArray{ PersonPopulator::GetHouseholdDistribution(country).oneMember,
											PersonPopulator::GetHouseholdDistribution(country).twoToThreeMembers,
											PersonPopulator::GetHouseholdDistribution(country).fourToFiveMembers,
											PersonPopulator::GetHouseholdDistribution(country).sixPlusMembers };

	size_t distIndex = GetDistributedArrayIndex(distributionArray);
	// Get a new index until the vector is not empty or the person is under twenty and the index is for one member homes
	while (homesByMemberCount.at(distIndex).empty() || (ageGroup <= Age_Group::UnderTwenty && distIndex == 0))
	{
		distIndex = GetDistributedArrayIndex(distributionArray);
	}
	// Return a random home of the chosen size
	return static_cast<Home*>(homesByMemberCount.at(distIndex).at(Random::RandomVectorIndex(homesByMemberCount.at(distIndex))));
}

DeseaseSpreadSimulation::Workplace* DeseaseSpreadSimulation::PersonPopulator::AssignWorkplace(const std::array<std::vector<Workplace*>, 5>& workplacesBySize) const
{
	// TODO: Implement Supply, HardwareStore and Morgue as a workplace. Currently ignored
	size_t distIndex = GetDistributedArrayIndex(Statistics::workplaceSize);
	// Get a new index until the vector is not empty
	while (workplacesBySize.at(distIndex).empty())
	{
		distIndex = GetDistributedArrayIndex(Statistics::workplaceSize);
	}
	// Return a random workplace at the chosen size
	return workplacesBySize.at(distIndex).at(Random::RandomVectorIndex(workplacesBySize.at(distIndex)));
}

void DeseaseSpreadSimulation::PersonPopulator::AssigneHomesToPopulation(std::vector<Person>& population, std::vector<Home>& homesToAssigne, Country country)
{
	std::vector<Home*> homes;
	for (auto& home : homesToAssigne)
	{
		homes.push_back(&home);
	}
	auto homesByMemberCount = PersonPopulator::HomesByMemberCount(population.size(), country, std::move(homes));

	for (auto& person : population)
	{
		person.SetHome(PersonPopulator::AssignHome(country, person.GetAgeGroup(), homesByMemberCount));
	}
}

std::vector<DeseaseSpreadSimulation::Statistics::HumanDistribution> DeseaseSpreadSimulation::PersonPopulator::GetCountryDistribution(Country country)
{
	switch (country)
	{
	case DeseaseSpreadSimulation::Country::USA:
		return Statistics::defaultAgeDistributionUSA;
		break;
	case DeseaseSpreadSimulation::Country::Germany:
		// TODO: Implement german distribution
		return Statistics::defaultAgeDistributionUSA;
		break;
	default:
		return Statistics::defaultAgeDistributionUSA;
		break;
	}
}

void DeseaseSpreadSimulation::PersonPopulator::AddCommunityToPopulation(Community* community, std::vector<Person>& population)
{
	for (auto& person : population)
	{
		person.SetCommunity(community);
	}
}

DeseaseSpreadSimulation::Statistics::HouseholdComposition DeseaseSpreadSimulation::PersonPopulator::GetHouseholdDistribution(Country country)
{
	switch (country)
	{
	case DeseaseSpreadSimulation::Country::USA:
		return Statistics::householdUSA;
		break;
	case DeseaseSpreadSimulation::Country::Germany:
		// TODO: Implement german distribution
		return Statistics::householdGermany;
		break;
	default:
		return Statistics::householdUSA;
		break;
	}
}
