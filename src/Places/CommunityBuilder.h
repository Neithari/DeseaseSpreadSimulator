#pragma once
#include "Places/Places.h"
#include "Person/PersonPopulator.h"

namespace DeseaseSpreadSimulation
{
	class CommunityBuilder
	{
	public:
		CommunityBuilder(size_t populationSize, Country country);
	private:
		std::shared_ptr<Home> AssignHomes() const;
		std::shared_ptr<Place> AssignWorkplace() const;

	private:
		PersonPopulator personFactory;

		std::vector<Person> population;
		std::vector<Home> homes;
	};
}
