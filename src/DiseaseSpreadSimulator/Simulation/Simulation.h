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
		void CompareContainmentMeasures(uint32_t runDays, uint32_t numberOfRuns = 1U);
		void Stop();
		void Pause();
		void Resume();

	private:
		void SetupEverything(uint32_t communityCount);
		static void InfectRandomPerson(const Disease* disease, std::vector<Person>& population);
		void SetupTravelInfecter(const Disease* disease, Community* community);
		void CreateCommunities(uint32_t communityCount);
		void ResetCommunities();

		void Update();
		void UpdatePopulation(std::vector<Person>& population);

		void Contacts(Places& places, Travel& travelLocation);
		static void ContactForPlace(Place& place);

		void Print() const;
		// Very verbose printing. Should only be used for debugging
		void PrintEveryHour() const; // cppcheck-suppress unusedPrivateFunction
		void PrintOncePerDay() const;
		void PrintPopulation(const std::vector<Person>& population) const;
		void PrintRunResult(const uint32_t days) const;

		bool CheckForNewDay();

		static void SetDiseaseContainmentMeasures(Community& community);

	private:
		bool m_withPrint{false};
		bool stop{true};
		bool pause{false};
		bool isSetupDone{false};

		static constexpr auto m_country{Country::USA};
		uint64_t m_populationSize{};
		const uint32_t m_initialPopulationSizeDigitCount{};
		TimeManager time{};
		std::vector<Community> communities{};
		std::vector<Disease> diseases{};

		Person travelInfecter;
		static constexpr auto minTravelContacts{0U};
		static constexpr auto maxTravelContacts{5U};
		mutable std::shared_mutex travelInfecterMutex{};
		mutable std::shared_mutex runNumberMutex{};
		mutable std::shared_mutex communitiesMutex{};

		uint64_t elapsedDays{};
		// We start with the first hour
		uint64_t elapsedHours{1U};
		bool isNewDay{false};

		uint32_t runNumber{};
		static constexpr uint32_t DiseaseContainmentMeasuresEnumSizePlusBase{5U};
	};
} // namespace DiseaseSpreadSimulation
