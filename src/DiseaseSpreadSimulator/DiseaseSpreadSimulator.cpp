#include <cstdint>
#include "Simulation/Simulation.h"

int main()
{
	constexpr uint64_t populationSize{5000U};
	constexpr uint32_t daysToRun{365U};

	DiseaseSpreadSimulation::Simulation simulation{populationSize, true};

	simulation.RunForDays(daysToRun);

	return 0;
}
