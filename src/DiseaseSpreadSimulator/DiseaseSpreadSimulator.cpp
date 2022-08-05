#include <cstdint>
#include "Simulation/Simulation.h"

int main()
{
	constexpr uint64_t populationSize{20000U};
	constexpr uint32_t daysToRun{365U};
	constexpr uint32_t numberOfRuns{3U};

	DiseaseSpreadSimulation::Simulation simulation{populationSize, true};

	simulation.CompareContainmentMeasures(daysToRun, numberOfRuns);

	return 0;
}
