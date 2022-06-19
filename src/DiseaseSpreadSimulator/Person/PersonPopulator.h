#pragma once
#include <random>
#include <vector>
#include "Enums.h"
#include "Statistics.h"
#include "Places/Places.h"
#include "RandomNumbers.h"

namespace DiseaseSpreadSimulation
{
	class Person;
	class Community;

	class PersonPopulator
	{
	public:
		PersonPopulator(const size_t populationSize, std::vector<Statistics::HumanDistribution> humanDistribution);

		std::vector<Person> CreatePopulation(Country country, std::vector<Home>& homes, std::vector<Workplace>& workplaces, std::vector<School>& schools, Community* community = nullptr);

		static size_t WorkingPeopleCount(const size_t populationSize, const Country country);
		static float WorkingPeopleCountFloat(const size_t populationSize, const Country country);
		static size_t SchoolKidsCount(const size_t populationSize, const Country country);
		static std::array<std::vector<Home*>, 4> HomesByMemberCount(const size_t populationSize, const Country country, std::vector<Home*> homes);
		static Home* AssignHome(const Country country, const Age_Group ageGroup, const std::array<std::vector<Home*>, 4>& homesByMemberCount);

		static void AddCommunityToPopulation(Community* community, std::vector<Person>& population);

		static Statistics::HouseholdComposition GetHouseholdDistribution(Country country);
		static std::vector<Statistics::HumanDistribution> GetCountryDistribution(Country country);

	private:
		// Get a new person for the chosen distribution.
		Person GetNewPerson(Community* community = nullptr, Home* home = nullptr);

		// Returns a rounded down percentage of count
		static size_t DistributionToCountHelper(size_t count, float percent);

		static void AssigneHomesToPopulation(std::vector<Person>& population, std::vector<Home>& homesToAssigne, Country country);
		static Workplace* AssignWorkplace(const std::array<std::vector<Workplace*>, 5>& workplacesBySize);

		// Returns an index weighted by the given distribution
		template <typename T, size_t SIZE>
		static size_t GetDistributedArrayIndex(const std::array<T, SIZE>& distributionArray)
		{
			// Create the distribution with the distributionArray as weights
			std::discrete_distribution<size_t> distribution(distributionArray.cbegin(), distributionArray.cend());
			return distribution(Random::generator);
		}

	private:
		const size_t m_populationSize{};
		bool m_allAssigned = false;
		size_t m_leftover{};
		bool m_lastFew = false;
		const std::vector<Statistics::HumanDistribution> m_ageDistribution;
		// Set to 1 because we use the distribution on 0 inside the constructor
		size_t m_ageDistributionIndex{};
		Statistics::HumanDistribution m_currentHumanDistribution;
		size_t m_currentHumanCount{};
	};
} // namespace DiseaseSpreadSimulation
