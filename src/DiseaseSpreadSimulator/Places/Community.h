#pragma once
#include <algorithm>
#include <random>
#include "Disease/DiseaseContainment.h"

namespace DiseaseSpreadSimulation
{

	class Community
	{
	public:
		Community(const size_t populationSize, const Country country);
		Community(const Community& other);
		Community(Community&& other) noexcept;
		Community& operator=(const Community& other);
		Community& operator=(Community&& other) noexcept;
		~Community() = default;

		void AddPerson(Person person);
		void RemovePerson(const Person& personToRemove);

		void AddPlaces(Places places);
		void AddPopulation(std::vector<Person>& population);

		std::optional<Person> TransferPerson(const Person& traveler);

		Place* TransferToHome(Person* person);
		Place* TransferToSupplyStore(Person* person);
		Place* TransferToHardwareStore(Person* person);
		Place* TransferToWork(Person* person);
		Place* TransferToSchool(Person* person);
		Place* TransferToMorgue(Person* person);
		Place* TransferToTravelLocation(Person* person);

		std::vector<Person>& GetPopulation();
		Places& GetPlaces();
		Travel& GetTravelLocation();
		std::vector<Home>& GetHomes();
		// Returns a random supply store
		Supply* GetSupplyStore();
		// Returns a random hardware store
		HardwareStore* GetHardwareStore();
		// Returns a random morgue
		Morgue* GetMorgue();

		void AddPlace(Home home);
		void AddPlace(Supply store);
		void AddPlace(Workplace workplace);
		void AddPlace(School school);
		void AddPlace(HardwareStore store);
		void AddPlace(Morgue morgue);

		const DiseaseContainment& ContainmentMeasures() const;
		void TestStation(Person* person);

	private:
		bool TestPersonForInfection(const Person* person) const;
		Place* TransferToPlace(Person* person, Place* place);

	private:
		std::vector<Person> m_population{};
		Places m_places{};
		Travel m_travelLocation{};
		DiseaseContainment m_containmentMeasures{};

		std::shared_timed_mutex populationMutex;
		std::shared_timed_mutex placesMutex;
	};
} // namespace DiseaseSpreadSimulation
