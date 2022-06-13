#include "Person/PersonPopulator.h"
#include "Person/Person.h"
#include "Person/PersonBehavior.h"
#include "Places/Community.h"
#include "Places/PlaceBuilder.h"

DiseaseSpreadSimulation::PersonPopulator::PersonPopulator(const size_t populationSize, std::vector<Statistics::HumanDistribution> humanDistribution)
	: m_populationSize(populationSize),
	  m_leftover(populationSize),
	  m_ageDistribution(std::move(humanDistribution)),
	  m_currentHumanDistribution(m_ageDistribution.front())
{
	// Set the currentHumanCount to a percent of the population and to 1 if this will return 0
	m_currentHumanCount = DistributionToCountHelper(populationSize, m_currentHumanDistribution.percent);
	if (m_currentHumanCount == 0)
	{
		m_currentHumanCount = 1;
	}
}

std::vector<DiseaseSpreadSimulation::Person> DiseaseSpreadSimulation::PersonPopulator::CreatePopulation(Country country, std::vector<Home>& homes, std::vector<Workplace>& workplaces, std::vector<School>& schools, Community* community)
{
	std::vector<Person> population;
	// Return early if the population is 0
	if (m_populationSize == 0)
	{
		return population;
	}

	population.reserve(m_populationSize);

	// This bool will ensure that a workplace is only assigned if there are workplaces
	bool noWorkplace = workplaces.empty();

	// Create an array containing all workplaces sorted by size
	std::vector<Workplace*> sortedWorkplaces;
	sortedWorkplaces.reserve(workplaces.size());

	std::transform(workplaces.begin(), workplaces.end(), std::back_inserter(sortedWorkplaces), [](auto& workplace)
		{
			return &workplace;
		});

	auto workplacesBySize(PlaceBuilder::WorkplacesBySize(m_populationSize, country, std::move(sortedWorkplaces)));

	// Create the population
	size_t schoolIndex = 0;
	auto averageSchoolSize = Statistics::AverageSchoolSize(country);
	while (!m_allAssigned)
	{
		// Get a new person
		auto person = GetNewPerson(community);

		// Assigne a workplace when the person is in working age and there are workplaces
		if (!noWorkplace && person.GetAgeGroup() > Age_Group::UnderTwenty && person.GetAgeGroup() <= Age_Group::UnderSeventy)
		{
			person.SetWorkplace(AssignWorkplace(workplacesBySize));
		}
		// Assign a school for every person under twenty
		else if (person.GetAgeGroup() <= Age_Group::UnderTwenty)
		{
			// Assign the school at the index until we reach the average school size
			if (averageSchoolSize-- > 0)
			{
				person.SetSchool(&schools.at(schoolIndex));
			}
			else
			{
				// Reset the size and advance school index
				averageSchoolSize = Statistics::AverageSchoolSize(country);
				schoolIndex++;
				// The last school will get all remaining school kids
				if (schoolIndex == schools.size())
				{
					schoolIndex = schools.size() - 1;
				}
			}
		}
		// Add the created person to the community
		population.push_back(std::move(person));
	}

	AssigneHomesToPopulation(population, homes, country);

	return population;
}

DiseaseSpreadSimulation::Person DiseaseSpreadSimulation::PersonPopulator::GetNewPerson(Community* community, Home* home)
{
	// As long as we don't have assigned the full population create a new person with age and sex according to our distribution
	// When the currentHumanCount is 0...
	if (m_currentHumanCount == 0)
	{
		// ...we advance the index and check if we used all distributions...
		if (++m_ageDistributionIndex >= m_ageDistribution.size())
		{
			// ...if yes we start again at the beginning...
			m_ageDistributionIndex = 0;
			// ...and distribute 1 to every human distribution until no leftover
			m_lastFew = true;
		}
		// ...we set the new current distribution...
		m_currentHumanDistribution = m_ageDistribution.at(m_ageDistributionIndex);

		if (!m_lastFew)
		{
			// ...set the currentHumanCount to a percent of the population...
			m_currentHumanCount = DistributionToCountHelper(m_populationSize, m_currentHumanDistribution.percent);
		}
		// ...and to 1 if this will return 0...
		if (m_currentHumanCount == 0)
		{
			m_currentHumanCount = 1;
		}
	}

	m_leftover--;
	m_currentHumanCount--;
	if (m_leftover == 0)
	{
		m_allAssigned = true;
	}

	return Person(m_currentHumanDistribution.ageGroup, m_currentHumanDistribution.sex, PersonBehavior(), community, home);
}

size_t DiseaseSpreadSimulation::PersonPopulator::WorkingPeopleCount(const size_t populationSize, const Country country)
{
	// TODO: Need a better way to get the working people. Not in sync with PersonPopulator::GetNewPerson()
	auto countryDistribution = std::move(GetCountryDistribution(country));
	// Sum up every human distribution inside working age (>20 and <70).
	return std::accumulate(countryDistribution.begin(), countryDistribution.end(), static_cast<size_t>(0), [populationSize](size_t people, const DiseaseSpreadSimulation::Statistics::HumanDistribution& humanDistribution)
		{
			if (humanDistribution.ageGroup > Age_Group::UnderTwenty && humanDistribution.ageGroup <= Age_Group::UnderSeventy)
			{
				return people += static_cast<size_t>(std::llround(static_cast<double>(populationSize) * static_cast<double>(humanDistribution.percent)));
			}
			return people;
		});
}

float DiseaseSpreadSimulation::PersonPopulator::WorkingPeopleCountFloat(const size_t populationSize, const Country country)
{
	return static_cast<float>(WorkingPeopleCount(populationSize, country));
}

size_t DiseaseSpreadSimulation::PersonPopulator::SchoolKidsCount(const size_t populationSize, const Country country)
{
	auto countryDistribution = std::move(GetCountryDistribution(country));
	// Sum up every human distribution inside school age (<20).
	return std::accumulate(countryDistribution.begin(), countryDistribution.end(), static_cast<size_t>(0), [populationSize](size_t people, const DiseaseSpreadSimulation::Statistics::HumanDistribution& humanDistribution)
		{
			if (humanDistribution.ageGroup <= Age_Group::UnderTwenty)
			{
				return people += static_cast<size_t>(std::llround(static_cast<double>(populationSize) * static_cast<double>(humanDistribution.percent)));
			}
			return people;
		});
}

std::array<std::vector<DiseaseSpreadSimulation::Home*>, 4> DiseaseSpreadSimulation::PersonPopulator::HomesByMemberCount(const size_t populationSize, const Country country, std::vector<Home*> homes)
{
	auto homeCounts(PlaceBuilder::GetHomeCounts(static_cast<float>(populationSize), country));

	// Set iterators to copy a part of the homes vector into the new vectores separated by size
	auto from = homes.cbegin();
	auto to = homes.cbegin() + static_cast<int64_t>(homeCounts.at(0));
	std::vector<Home*> oneMember(from, to);
	// Change the iterators for the next size and do the same for every size
	from = to;
	to += static_cast<int64_t>(homeCounts.at(1));
	std::vector<Home*> twoToThreeMembers(from, to);
	from = to;
	to += static_cast<int64_t>(homeCounts.at(2));
	std::vector<Home*> fourToFiveMembers(from, to);
	from = to;
	std::vector<Home*> sixPlusMembers(from, homes.cend());

	// Return an array with all size vectors
	return {oneMember, twoToThreeMembers, fourToFiveMembers, sixPlusMembers};
}

size_t DiseaseSpreadSimulation::PersonPopulator::DistributionToCountHelper(size_t count, float percent)
{
	// Scale count by percent and then omit the decimal
	return static_cast<size_t>(static_cast<double>(count) * static_cast<double>(percent));
}

DiseaseSpreadSimulation::Home* DiseaseSpreadSimulation::PersonPopulator::AssignHome(const Country country, const Age_Group ageGroup, const std::array<std::vector<Home*>, 4>& homesByMemberCount)
{
	// Create the distribution
	std::array<double, 4> distributionArray{PersonPopulator::GetHouseholdDistribution(country).oneMember,
		PersonPopulator::GetHouseholdDistribution(country).twoToThreeMembers,
		PersonPopulator::GetHouseholdDistribution(country).fourToFiveMembers,
		PersonPopulator::GetHouseholdDistribution(country).sixPlusMembers};

	size_t distIndex = GetDistributedArrayIndex(distributionArray);
	// Get a new index until the vector is not empty or the person is under twenty and the index is for one member homes
	while (homesByMemberCount.at(distIndex).empty() || (ageGroup <= Age_Group::UnderTwenty && distIndex == 0))
	{
		distIndex = GetDistributedArrayIndex(distributionArray);
	}
	// Return a random home of the chosen size
	return static_cast<Home*>(homesByMemberCount.at(distIndex).at(Random::RandomVectorIndex(homesByMemberCount.at(distIndex))));
}

DiseaseSpreadSimulation::Workplace* DiseaseSpreadSimulation::PersonPopulator::AssignWorkplace(const std::array<std::vector<Workplace*>, 5>& workplacesBySize) const
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

void DiseaseSpreadSimulation::PersonPopulator::AssigneHomesToPopulation(std::vector<Person>& population, std::vector<Home>& homesToAssigne, Country country)
{
	std::vector<Home*> homes;
	homes.reserve(homesToAssigne.size());
	std::transform(homesToAssigne.begin(), homesToAssigne.end(), std::back_inserter(homes), [](auto& home)
		{
			return &home;
		});

	auto homesByMemberCount = PersonPopulator::HomesByMemberCount(population.size(), country, std::move(homes));

	for (auto& person : population)
	{
		person.SetHome(PersonPopulator::AssignHome(country, person.GetAgeGroup(), homesByMemberCount));
	}
}

std::vector<DiseaseSpreadSimulation::Statistics::HumanDistribution> DiseaseSpreadSimulation::PersonPopulator::GetCountryDistribution(Country country)
{
	switch (country)
	{
	case DiseaseSpreadSimulation::Country::USA:
		return Statistics::defaultAgeDistributionUSA;
		break;
	case DiseaseSpreadSimulation::Country::Germany:
		// TODO: Implement german distribution
		return Statistics::defaultAgeDistributionUSA;
		break;
	default:
		return Statistics::defaultAgeDistributionUSA;
		break;
	}
}

void DiseaseSpreadSimulation::PersonPopulator::AddCommunityToPopulation(Community* community, std::vector<Person>& population)
{
	for (auto& person : population)
	{
		person.SetCommunity(community);
	}
}

DiseaseSpreadSimulation::Statistics::HouseholdComposition DiseaseSpreadSimulation::PersonPopulator::GetHouseholdDistribution(Country country)
{
	switch (country)
	{
	case DiseaseSpreadSimulation::Country::USA:
		return Statistics::householdUSA;
		break;
	case DiseaseSpreadSimulation::Country::Germany:
		// TODO: Implement german distribution
		return Statistics::householdGermany;
		break;
	default:
		return Statistics::householdUSA;
		break;
	}
}
