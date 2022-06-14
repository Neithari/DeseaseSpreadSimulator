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

		void Print();
		// Very verbose printing. Should only be used for debugging
		void PrintEveryHour(); // cppcheck-suppress unusedPrivateFunction
		void PrintOncePerDay();
		static void PrintPopulation(const std::vector<Person>& population);

		bool CheckForNewDay();

	private:
		bool m_withPrint = false;
		bool stop = true;
		bool pause = false;

		static constexpr Country country = Country::USA;
		uint64_t m_populationSize{};
		TimeManager time;
		std::vector<Community> communities;
		std::vector<Disease> diseases;

		Person travelInfecter;
		std::shared_timed_mutex travelInfecterMutex;

		uint64_t elapsedDays{};
		// We start with the first hour
		uint64_t elapsedHours{1u};
		bool isNewDay = false;
	};
} // namespace DiseaseSpreadSimulation
