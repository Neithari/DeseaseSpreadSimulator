#include "pch.h"
#include "Simulation.h"

DeseaseSpreadSimulation::Simulation::Simulation(Desease desease, bool withPrint)
	:
	desease(std::move(desease)),
	withPrint(withPrint)
{
	persons.reserve(personCount);
}

void DeseaseSpreadSimulation::Simulation::Start()
{
}

void DeseaseSpreadSimulation::Simulation::Update()
{
	if (withPrint)
	{
		Print();
	}
}

void DeseaseSpreadSimulation::Simulation::Print()
{
}
