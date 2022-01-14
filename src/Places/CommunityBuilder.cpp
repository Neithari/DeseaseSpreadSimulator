#include "pch.h"
#include "Places/CommunityBuilder.h"
#include "Person/PersonPopulator.h"
#include "Places/PlaceBuilder.h"

DeseaseSpreadSimulation::Community DeseaseSpreadSimulation::CommunityBuilder::CreateCommunity(const size_t populationSize, const Country country) const
{
	Community community;
	PlaceBuilder placeFactory;
	PersonPopulator populationFactory(populationSize, PersonPopulator::GetCountryDistribution(country));

	community.AddPlaces(placeFactory.CreatePlaces(populationSize, country));

	community.AddPopulation(populationFactory.CreatePopulation(populationSize, country, &community, community.GetPlaces()));

	return community;
}
