#include "pch.h"
#include "Simulation/Simulation.h"

DeseaseSpreadSimulation::Simulation::Simulation(Desease desease, bool withPrint)
	:
	desease(std::move(desease)),
	withPrint(withPrint)
{
	persons.reserve(personCount);
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
	simulationTime.SetSimulationTimeMultiplier(std::move(multiplier));
}

void DeseaseSpreadSimulation::Simulation::Update()
{
	simulationTime.Update();

	if (withPrint)
	{
		Print();
	}
}

void DeseaseSpreadSimulation::Simulation::Print()
{
}
