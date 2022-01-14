#include "pch.h"
#include "Simulation/Simulation.h"

int main()
{	
	DeseaseSpreadSimulation::Simulation simulation{1000, true};

	simulation.Run();

	return 0;
}
