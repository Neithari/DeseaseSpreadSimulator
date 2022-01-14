#include "pch.h"
#include "Simulation/Simulation.h"

int main()
{	
	DeseaseSpreadSimulation::Simulation simulation{1000, true};

	simulation.SetSimulationSpeedMultiplier(2);
	simulation.Run();

	return 0;
}
