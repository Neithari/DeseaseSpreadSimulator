#pragma once

namespace DiseaseSpreadSimulation
{
	namespace PlaceBuilder
	{
		Places CreatePlaces(const size_t populationSize, const Country country);
		std::array<std::vector<Workplace*>, 5> WorkplacesBySize(const size_t populationSize, const Country country, std::vector<Workplace*> workplaces);
		std::array<size_t, 4> GetHomeCounts(const float populationSize, const Country country);
	} // namespace PlaceBuilder
} // namespace DiseaseSpreadSimulation
