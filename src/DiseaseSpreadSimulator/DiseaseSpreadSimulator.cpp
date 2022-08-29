#include "CommandParser.h"
#include "Simulation/Simulation.h"

int main(int argc, char* argv[])
{
	DiseaseSpreadSimulation::CommandParser commands{argc, argv};

	DiseaseSpreadSimulation::Simulation simulation{commands.GetPopulationSize(), commands.GetWithPrint(), commands.GetDiseaseFilename(), commands.GetCountry()};

	simulation.CompareContainmentMeasures(commands.GetDaysToRun(), commands.GetNumberOfRuns());

	return 0;
}
