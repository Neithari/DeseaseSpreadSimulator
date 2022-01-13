#pragma once
#include <algorithm>
#include <random>

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

		const std::vector<Home*>& GetHomes();
		// Returns a random store
		Supply* GetSupplyStore();
		// Returns a random store
		HardwareStore* GetHardwareStore();
		// Returns a random morgue
		Morgue* GetMorgue();

	private:
		// Transfer the contends from source to target.
		template <typename T, typename A>
		void TransferUniquePtrVector(std::vector<std::unique_ptr<T, A>> source, std::vector<std::unique_ptr<T, A>>& target)
		{
			std::move(source.begin(), source.end(), std::back_inserter(target));
		};
		// Add a pointer of type home, supply, hardware store and morgue to matching vectors for easier lookup
		void AddPlaceToLookupVector(Place* place);

		// Returns an random index from 0 to size - 1
		template <typename T>
		T RandomIndex(T size) const
		{
			// Prevent return of a negative index
			if (size <= (T)0)
			{
				return (T)0;
			}

			std::random_device seed;
			std::mt19937 generator(seed());
			std::uniform_int_distribution distribution((T)0, size - (T)1);

			return distribution(generator);
		};

	private:
		// TODO: Refactor the vectors from pointers/unique_ptrs to different vectors for every place
		// -----------------------------------------------------------------------------------------
		std::vector<std::unique_ptr<Person>> m_population;
		std::vector<std::unique_ptr<Place>> m_places;

		std::vector<Home*> homes;
		std::vector<Supply*> supplyStores;
		std::vector<HardwareStore*> hardwareStores;
		std::vector<Morgue*> morgues;
		// -----------------------------------------------------------------------------------------
	};
}
