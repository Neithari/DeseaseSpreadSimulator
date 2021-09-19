#pragma once
#include "Places/Places.h"
#include "Person/PersonPopulator.h"

namespace DeseaseSpreadSimulation
{
	class Community;

	class CommunityBuilder
	{
	public:
		CommunityBuilder() = default;

		Community CreateCommunity(size_t populationSize, Country country) const;
	private:
		void CreatePlaces(size_t populationSize, Country country, Community& outCommunity) const;
		void CreatePopulation(size_t populationSize, Country country, Community& outCommunity) const;
		Home* AssignHome() const;
		Place* AssignWorkplace() const;
		// Helper functions for place and population creation
		size_t WorkingPeopleNumber(size_t populationSize, Country country) const;
		size_t GetHomeCount(size_t populationSize, Country country) const;
	private:
		// Workplace size estimates https://www.statista.com/statistics/944669/current-office-size-full-time-employees-usa/
		static constexpr std::array<float, 5> workplaceSize{ 0.2649f, 0.308f, 0.1908f, 0.0821f, 0.1542f };
	};
}
