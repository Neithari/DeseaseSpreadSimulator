#pragma once
#include <cstdint>
#include <vector>
#include <shared_mutex>
#include "Enums.h"
#include "Simulation/TimeManager.h"
#include "Person/Person.h"
#include "Disease/Disease.h"
#include "Places/Community.h"

namespace DiseaseSpreadSimulation
{
	class Simulation
	{
	public:
		explicit Simulation(uint64_t populationSize, bool withPrint);

		void Run();
		// Will run the simulation for the stated days and print a result after
		void RunForDays(uint32_t days);
		void Stop();
		void Pause();
		void Resume();

	private:
		void SetupEverything(uint32_t communityCount);
		static void InfectRandomPerson(const Disease* disease, std::vector<Person>& population);
		void SetupTravelInfecter(const Disease* disease, Community* communitie);

		void Update();
		void UpdatePopulation(std::vector<Person>& population);

		void Contacts(Places& places, Travel& travelLocation);
		static void ContactForPlace(Place& place);

		void Print() const;
		// Very verbose printing. Should only be used for debugging
		void PrintEveryHour() const; // cppcheck-suppress unusedPrivateFunction
		void PrintOncePerDay() const;
		void PrintPopulation(const std::vector<Person>& population) const;
		void PrintRunResult(const uint32_t runNumber, const uint32_t days) const;

		bool CheckForNewDay();

	private:
		bool m_withPrint{false};
		bool stop{true};
		bool pause{false};

		static constexpr auto m_country{Country::USA};
		static constexpr auto m_communityCount{1U};
		uint64_t m_populationSize{};
		const uint32_t m_initialPopulationSizeDigitCount{};
		TimeManager time{};
		std::vector<Community> communities{};
		std::vector<Disease> diseases{};

		Person travelInfecter;
		static constexpr auto minTravelContacts{0U};
		static constexpr auto maxTravelContacts{5U};
		std::shared_mutex travelInfecterMutex{};

		uint64_t elapsedDays{};
		// We start with the first hour
		uint64_t elapsedHours{1U};
		bool isNewDay{false};
	};
} // namespace DiseaseSpreadSimulation
