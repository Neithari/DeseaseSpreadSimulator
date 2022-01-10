#pragma once
#include "DeseaseSpreadSimulator/Statistics.h"
#include <random>

namespace DeseaseSpreadSimulation
{	
	class PersonBehavior;

	class PersonPopulator
	{
	public:		
		PersonPopulator(size_t populationSize, std::vector<Statistics::HumanDistribution> humanDistribution);

		std::vector<std::unique_ptr<Person>> CreatePopulation(size_t populationSize, Country country, const Community& community, const std::vector<std::unique_ptr<Place>>& places);

		// Get a new person for the chosen distribution. Call should return a empty pointer when population size is reached
		std::unique_ptr<Person> GetNewPerson(const Community& community, Home* home = nullptr);

		static size_t WorkingPeopleCount(const size_t populationSize, const Country country);
		static std::array<std::vector<Home*>, 4> HomesByMemberCount(const size_t populationSize, const Country country, const std::vector<Home*>& homes);

		// Helper function to select the distribution inside the constructor initializer list
		static Statistics::HouseholdComposition GetHouseholdDistribution(Country country);
		static std::vector<Statistics::HumanDistribution> GetCountryDistribution(Country country);

	private:
		// Returns a rounded down percentage of count
		static size_t DistributionToCountHelper(size_t count, float percent);
		// Get a random index between 0 and maxIndex
		size_t GetUniformRandomIndex(size_t maxIndex) const;

		// Returns an index weighted by the given distribution
		template <typename T, size_t SIZE>
		static size_t GetDistributedArrayIndex(const std::array<T, SIZE>& distributionArray)
		{
			std::random_device seed;
			std::mt19937 generator(seed());
			// Create the distribution with the distributionArray as weights
			std::discrete_distribution<size_t> distribution(distributionArray.cbegin(), distributionArray.cend());
			return distribution(generator);
		};

		Home* AssignHome(const Country country, const Age_Group ageGroup, const std::array<std::vector<Home*>, 4>& homesByMemberCount) const;
		Workplace* AssignWorkplace(const std::array<std::vector<Workplace*>, 5>& workplacesBySize) const;

		// Returns an random index from 0 to size - 1
		template <typename T>
		T RandomIndex(T size) const
		{
			std::random_device seed;
			std::mt19937 generator(seed());
			std::uniform_int_distribution distribution((T)0, size - 1);

			return distribution(generator);
		};

		PersonBehavior CreatePersonBehavior() const;

	private:
		const size_t m_populationSize = 0;
		size_t leftover = 0;
		bool lastFew = false;
		const std::vector<Statistics::HumanDistribution> ageDistribution;
		// Set to 1 because we use the distribution on 0 inside the constructor
		size_t ageDistributionIndex = 1;
		Statistics::HumanDistribution currentHumanDistribution;
		size_t currentHumanCount = 0;
	};
}
