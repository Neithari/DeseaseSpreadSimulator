#include "pch.h"
#include "Simulation/Simulation.h"

DeseaseSpreadSimulation::Simulation::Simulation(bool withPrint)
	:
	withPrint(withPrint)
{
}

void DeseaseSpreadSimulation::Simulation::Start()
{
	stop = false;
	while (!stop)
	{
		while (pause)
		{
			// Idle when pause is called until resumed
		}

		Update();
	}
}

void DeseaseSpreadSimulation::Simulation::Stop()
{
	stop = true;
}

void DeseaseSpreadSimulation::Simulation::Pause()
{
	pause = true;
}

void DeseaseSpreadSimulation::Simulation::Resume()
{
	pause = false;
}

void DeseaseSpreadSimulation::Simulation::SetSimulationSpeedMultiplier(uint16_t multiplier)
{
	TimeManager::Instance().SetSimulationTimeMultiplier(multiplier);
}

void DeseaseSpreadSimulation::Simulation::Update()
{
	TimeManager::Instance().Update();

	if (withPrint)
	{
		Print();
	}
}

void DeseaseSpreadSimulation::Simulation::Print()
{
}
