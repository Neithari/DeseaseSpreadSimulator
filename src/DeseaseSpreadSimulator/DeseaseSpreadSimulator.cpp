#include "pch.h"
#include "Simulation/Simulation.h"

int main()
{	
	DeseaseSpreadSimulation::Simulation simulation{false};

	simulation.Run();

	return 0;
}
