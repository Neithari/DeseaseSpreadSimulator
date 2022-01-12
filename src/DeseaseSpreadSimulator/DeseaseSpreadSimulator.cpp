#include "pch.h"
#include "Simulation/Simulation.h"

int main()
{	
	DeseaseSpreadSimulation::Simulation simulation{false};

	simulation.Start();

	return 0;
}
