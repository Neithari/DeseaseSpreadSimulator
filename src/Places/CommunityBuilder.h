#pragma once
#include "Person/PersonPopulator.h"
#include <random>

namespace DeseaseSpreadSimulation
{
	class Community;

	class CommunityBuilder
	{
	public:
		CommunityBuilder() = default;

		Community CreateCommunity(const size_t populationSize, const Country country) const;
	private:
		void CreatePlaces(const size_t populationSize, const Country country, Community& outCommunity) const;
		void CreatePopulation(const size_t populationSize, const Country country, Community& outCommunity) const;
		Home* AssignHome(const Country country, const Age_Group ageGroup, const std::array<std::vector<Place*>, 4>& homesByMemberCount) const;
		Place* AssignWorkplace(const std::array<std::vector<Place*>, 5>& workplacesBySize) const;

		// Helper functions for place and population creation
		size_t WorkingPeopleNumber(const size_t populationSize, const Country country) const;
		std::array<size_t, 4> GetHomeCounts(const size_t populationSize, const Country country) const;
		std::array<std::vector<Place*>, 4> HomesByMemberCount(const size_t populationSize, const Country country, const std::vector<Place*>& homes) const;
		std::array<std::vector<Place*>, 5> WorkplacesBySize(const size_t populationSize, const Country country, std::vector<Place*> workplaces) const;
		// Get a random index between 0 and maxIndex
		size_t GetUniformRandomIndex(size_t maxIndex) const;
		
		// Returns an index weighted by the given distribution
		template <typename T, size_t SIZE>
		size_t GetDistributedArrayIndex(const std::array<T, SIZE>& distributionArray) const
		{
			// First choose a weighted workplace vector, then assigne uniform under the same size workplaces
			std::random_device seed;
			std::mt19937 generator(seed());
			// Create the distribution with the workplaceSize array as weights
			std::discrete_distribution<size_t> distribution(distributionArray.cbegin(), distributionArray.cend());
			return distribution(generator);
		}
	private:
		// Workplace size estimates https://www.statista.com/statistics/944669/current-office-size-full-time-employees-usa/
		static constexpr std::array<float, 5> workplaceSize{ 0.2649f, 0.308f, 0.1908f, 0.0821f, 0.1542f };
	};
}
