#pragma once

namespace DeseaseSpreadSimulation
{
	class PlaceBuilder
	{
	public:
		PlaceBuilder() = default;

		std::vector<std::unique_ptr<Place>> CreatePlaces(const size_t populationSize, const Country country) const;

		static std::array<std::vector<Place*>, 5> WorkplacesBySize(const size_t populationSize, const Country country, std::vector<Place*> workplaces);
		static std::array<size_t, 4> GetHomeCounts(const size_t populationSize, const Country country);
	};
}
