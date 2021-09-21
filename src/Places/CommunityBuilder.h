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

		Community CreateCommunity(const size_t populationSize, const Country country) const;
	private:
		void CreatePlaces(const size_t populationSize, const Country country, Community& outCommunity) const;
		void CreatePopulation(const size_t populationSize, const Country country, Community& outCommunity) const;
		Home* AssignHome(const size_t populationSize, const Country country) const;
		Place* AssignWorkplace() const;

		// Helper functions for place and population creation
		size_t WorkingPeopleNumber(const size_t populationSize, const Country country) const;
		std::array<size_t, 4> GetHomeCounts(const size_t populationSize, const Country country) const;
		std::array<std::vector<Place*>, 4> HomesByMemberCount(const size_t populationSize, const Country country, std::vector<Place*> homes) const;
		std::array<std::vector<Place*>, 5> WorkplacesBySize(const size_t populationSize, const Country country, std::vector<Place*> workplaces) const;
	private:
		// Workplace size estimates https://www.statista.com/statistics/944669/current-office-size-full-time-employees-usa/
		static constexpr std::array<float, 5> workplaceSize{ 0.2649f, 0.308f, 0.1908f, 0.0821f, 0.1542f };
	};
}
