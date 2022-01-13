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
		
		void Update();
		void Print();

		void Contacts(Community& community);

	private:
		bool withPrint = false;
		bool stop = true;
		bool pause = false;

		static constexpr Country country = Country::USA;
		uint16_t populationSize = 0;
		std::vector<Community> communities;
	};
}
