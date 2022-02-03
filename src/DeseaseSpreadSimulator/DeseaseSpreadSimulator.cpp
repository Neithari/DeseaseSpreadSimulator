#include "pch.h"
#include "Simulation/Simulation.h"

int main()
{	
	constexpr uint64_t populationSize = 100000u;

	DeseaseSpreadSimulation::Simulation simulation{populationSize, true};

	simulation.SetSimulationSpeedMultiplier(2);
	simulation.Run();

	return 0;
}
