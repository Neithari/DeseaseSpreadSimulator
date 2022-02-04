#include "pch.h"
#include "Places/CommunityBuilder.h"
#include "Person/PersonPopulator.h"
#include "Places/PlaceBuilder.h"

std::unique_ptr<DeseaseSpreadSimulation::Community> DeseaseSpreadSimulation::CommunityBuilder::CreateCommunity(const size_t populationSize, const Country country) const
{
	auto community = std::make_unique<Community>();
	PlaceBuilder placeFactory;
	PersonPopulator populationFactory(populationSize, PersonPopulator::GetCountryDistribution(country));

	community->AddPlaces(placeFactory.CreatePlaces(populationSize, country));

	auto population = populationFactory.CreatePopulation(populationSize, country, *community);
	community->AddPopulation(population);

	// BUGFIX: Bug der die pointer  invalidiert
	return community;

}
