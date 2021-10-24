#pragma once

namespace DeseaseSpreadSimulation
{
	class PlaceBuilder
	{
	public:
		std::vector<std::unique_ptr<Place>> CreatePlaces(const size_t populationSize, const Country country) const;
	private:
		size_t WorkingPeopleNumber(const size_t populationSize, const Country country) const;
		std::array<size_t, 4> GetHomeCounts(const size_t populationSize, const Country country) const;
		
		// Workplace size estimates https://www.statista.com/statistics/944669/current-office-size-full-time-employees-usa/
		static constexpr std::array<float, 5> workplaceSize{ 0.2649f, 0.308f, 0.1908f, 0.0821f, 0.1542f };
	};
}
