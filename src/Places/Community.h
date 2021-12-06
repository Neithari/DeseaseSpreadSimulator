#pragma once
#include <algorithm>

namespace DeseaseSpreadSimulation
{
	class Community
	{
	public:
		Community() = default;
		Community(std::vector<std::unique_ptr<Person>> population, std::vector<std::unique_ptr<Place>> places);

		void AddPlace(std::unique_ptr<Place> place);
		void AddPlaces(std::vector<std::unique_ptr<Place>> places);
		void AddPerson(std::unique_ptr<Person> person);
		void AddPopulation(std::vector<std::unique_ptr<Person>> population);
		
		void RemovePlace(uint32_t placeID);
		void RemovePerson(uint32_t personID);
		
		std::unique_ptr<Person> TransferPerson(uint32_t personID);
		
		std::vector<std::unique_ptr<Person>>& GetPopulation();
		std::vector<std::unique_ptr<Place>>& GetPlaces();

	private:
		// Transfer the contends from source to target.
		template <typename T, typename A>
		void TransferUniquePtrVector(std::vector<std::unique_ptr<T, A>> source, std::vector<std::unique_ptr<T, A>>& target)
		{
			std::move(source.begin(), source.end(), std::back_inserter(target));
		};

	private:
		std::vector<std::unique_ptr<Person>> m_population;
		std::vector<std::unique_ptr<Place>> m_places;
	};
}
