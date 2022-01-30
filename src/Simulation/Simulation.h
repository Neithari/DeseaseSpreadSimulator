#pragma once
#include "Simulation/TimeManager.h"

namespace DeseaseSpreadSimulation
{
	class Simulation
	{
	public:
		Simulation(uint64_t populationSize, bool withPrint = false);

		void Run();
		void Stop();
		void Pause();
		void Resume();

		void SetSimulationSpeedMultiplier(uint16_t multiplier);

	private:
		void SetupEverything(uint16_t communityCount);
		void InfectRandomPerson(const Desease* desease, std::vector<Person>& population);
		
		void Update();
		void Print();
		void PrintEveryHour();
		void PrintOncePerDay();
		void PrintPopulation(const std::vector<Person>& population) const;


		void Contacts(Community& community);
		void ContactForPlace(Place& place);

	private:
		bool withPrint = false;
		bool stop = true;
		bool pause = false;

		static constexpr Country country = Country::USA;
		uint64_t populationSize = 0u;
		std::vector<Community> communities;
		std::vector<Desease> deseases;

		uint64_t elapsedTime = 0u;
	};
}
