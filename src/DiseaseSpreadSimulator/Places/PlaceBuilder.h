#pragma once

namespace DiseaseSpreadSimulation
{
	class PlaceBuilder
	{
	public:
		PlaceBuilder() = default;

		Places CreatePlaces(const size_t populationSize, const Country country) const;

		static std::array<std::vector<Workplace*>, 5> WorkplacesBySize(const size_t populationSize, const Country country, std::vector<Workplace*> workplaces);
		static std::array<size_t, 4> GetHomeCounts(const size_t populationSize, const Country country);
	};
}
