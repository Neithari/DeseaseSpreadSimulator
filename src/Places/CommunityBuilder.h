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
	};
}
