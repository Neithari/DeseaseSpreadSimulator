#pragma once
#include "Simulation/TimeManager.h"

namespace DeseaseSpreadSimulation
{
	class Simulation
	{
	public:
		Simulation(bool withPrint = false);

		void Start();
		void Stop();
		void Pause();
		void Resume();

		void SetSimulationSpeedMultiplier(uint16_t multiplier);

	private:
		void Update();
		void Print();

	private:
		bool withPrint = false;
		bool stop = true;
		bool pause = false;

		unsigned int personCount = 1000;
	};
}
