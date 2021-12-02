#include "pch.h"
#include "Places/CommunityBuilder.h"
#include "Places/Community.h"
#include "Person/PersonPopulator.h"
#include "Places/PlaceBuilder.h"

DeseaseSpreadSimulation::Community DeseaseSpreadSimulation::CommunityBuilder::CreateCommunity(const size_t populationSize, const Country country) const
{
	Community community;
	PlaceBuilder placeFactory;
	PersonPopulator populationFactory(populationSize, PersonPopulator::GetCountryDistribution(country));

	community.AddPlaces(std::move(placeFactory.CreatePlaces(populationSize, country)));

	community.AddPopulation(std::move(populationFactory.CreatePopulation(populationSize, country, community.GetPlaces())));

	return community;
}
