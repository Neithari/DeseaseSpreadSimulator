#pragma once
#include "Simulation/TimeManager.h"

namespace DeseaseSpreadSimulation
{
	class Simulation
	{
	public:
		Simulation(uint16_t populationSize, bool withPrint = false);

		void Run();
		void Stop();
		void Pause();
		void Resume();

		void SetSimulationSpeedMultiplier(uint16_t multiplier);

	private:
		void SetupEverything(uint16_t communityCount);
		void InfectRandomPerson(const Desease* desease, std::vector<std::unique_ptr<Person>>& population);
		
		void Update();
		void Print();
		void PrintEveryHour();
		void PrintOncePerDay();

		void Contacts(Community& community);

	private:
		bool withPrint = false;
		bool stop = true;
		bool pause = false;

		static constexpr Country country = Country::USA;
		uint16_t populationSize = 0;
		std::vector<Community> communities;
		std::vector<Desease> deseases;

		uint64_t elapsedTime = 0;
	};
}
