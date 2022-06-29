#pragma once
#include <cstdint>
#include <optional>
#include <vector>
#include <algorithm>
#include <random>
#include <shared_mutex>
#include "Disease/DiseaseContainment.h"
#include "Places/Places.h"

namespace DiseaseSpreadSimulation
{
	class Person;

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
		const std::vector<Person>& GetPopulation() const;
		Places& GetPlaces();
		const Places& GetPlaces() const;
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

		DiseaseContainment& SetContainmentMeasures();
		const DiseaseContainment& ContainmentMeasures() const;
		void TestStation(Person* person);

		[[nodiscard]] uint32_t GetID() const;

		[[nodiscard]] size_t CurrentInfectionMax() const;
		[[nodiscard]] size_t NumberOfPositiveTests() const;
		[[nodiscard]] size_t NumberOfPersonsQuarantined() const;

	private:
		static bool TestPersonForInfection(const Person* person);
		Place* TransferToPlace(Person* person, Place* place);

	private:
		const uint32_t m_id{0};
		std::vector<Person> m_population{};
		Places m_places{};
		Travel m_travelLocation;
		DiseaseContainment m_containmentMeasures{};

		size_t m_positiveTests{0};
		size_t m_personsQuarantined{0};

		mutable std::shared_mutex populationMutex;
		mutable std::shared_mutex placesMutex;
		mutable std::shared_mutex testStationMutex;
	};
} // namespace DiseaseSpreadSimulation
