#include <cstdint>
#include "Simulation/Simulation.h"

int main()
{
	constexpr uint64_t populationSize = 5000U;

	DiseaseSpreadSimulation::Simulation simulation{populationSize, true};

	simulation.Run();

	return 0;
}
