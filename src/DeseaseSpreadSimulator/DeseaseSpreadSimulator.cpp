#include "pch.h"
#include "Simulation/Simulation.h"

int main()
{	
	constexpr uint64_t populationSize = 5000u;

	DeseaseSpreadSimulation::Simulation simulation{populationSize, true};

	simulation.Run();

	return 0;
}
