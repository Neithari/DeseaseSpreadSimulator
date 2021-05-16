#include "pch.h"
#include "Places/CommunityBuilder.h"

DeseaseSpreadSimulation::CommunityBuilder::CommunityBuilder(size_t populationSize, DeseaseSpreadSimulation::Country country)
	:
	personFactory(populationSize, PersonPopulator::GetCountryDistribution(country))
{
	population.reserve(populationSize);
}
