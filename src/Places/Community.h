#pragma once
#include <algorithm>
#include <random>

namespace DeseaseSpreadSimulation
{
	class Community
	{
	public:
		Community() = default;
		Community(std::vector<Person> population, Places places);

		void AddPlace(Home home);
		void AddPlace(Supply store);
		void AddPlace(Workplace workplace);
		void AddPlace(School school);
		void AddPlace(HardwareStore store);
		void AddPlace(Morgue morgue);
		void RemovePlace(const Home& home);
		void RemovePlace(const Supply& store);
		void RemovePlace(const Workplace& workplace);
		void RemovePlace(const School& school);
		void RemovePlace(const HardwareStore& store);
		void RemovePlace(const Morgue& morgue);

		void AddPerson(Person person);
		void RemovePerson(uint32_t personID);

		void AddPlaces(Places places);
		void AddPopulation(std::vector<Person> population);
		
		Person TransferPerson(uint32_t personID);
		
		std::vector<Person>& GetPopulation();
		Places& GetPlaces();

		std::vector<Home>& GetHomes();
		// Returns a random supply store
		Supply* GetSupplyStore();
		// Returns a random hardware store
		HardwareStore* GetHardwareStore();
		// Returns a random morgue
		Morgue* GetMorgue();

	private:
		// Returns an random index from 0 to size - 1
		template <typename T>
		T RandomIndex(T size) const
		{
			// Prevent return of a negative index
			if (size <= (T)0)
			{
				throw(std::out_of_range("Size = 0. Container possibly empty!"));
			}

			std::random_device seed;
			std::mt19937 generator(seed());
			std::uniform_int_distribution distribution((T)0, size - (T)1);

			return distribution(generator);
		};

	private:
		std::vector<Person> m_population;
		Places m_places;
	};
}
