#pragma once
#include "Simulation/TimeManager.h"

namespace DeseaseSpreadSimulation
{
	class Simulation
	{
	public:
		Simulation(Desease desease, bool withPrint = false);

		void Start();
		void Stop();
		void Pause();
		void Resume();

		void SetSimulationSpeedMultiplier(uint16_t multiplier);

	private:
		void Update();
		void Print();

	private:
		Desease desease;
		bool withPrint = false;
		bool stop = true;
		bool pause = false;
		
		TimeManager simulationTime;

		unsigned int personCount = 1000;
		std::vector<Person> persons;
	};
}
