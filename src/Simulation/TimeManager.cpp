#include "pch.h"
#include "Simulation/TimeManager.h"

DeseaseSpreadSimulation::TimeManager::TimeManager()
	:
	currentFrameTime(std::chrono::steady_clock::now()),
	lastFrameTime(currentFrameTime)
{
}

void DeseaseSpreadSimulation::TimeManager::Update()
{
	// update the frame times...
	lastFrameTime = currentFrameTime;
	currentFrameTime = std::chrono::steady_clock::now();
	// ...save the duration of the last frame
	frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - lastFrameTime);

	// sum the time the simulation is running and scale it with the multiplier
	frameSum += static_cast<uint32_t>(frameTime.count()) * simulationTimeMultiplier;
	// while the sum > tick advance the sumulationTime
	while (frameSum >= tick)
	{
		simulationTime++;
		frameSum -= tick;
	}
}
int64_t DeseaseSpreadSimulation::TimeManager::GetFrameTime() const
{
	return frameTime.count();
}

uint64_t DeseaseSpreadSimulation::TimeManager::GetElapsedDays() const
{
	return simulationTime;
}

void DeseaseSpreadSimulation::TimeManager::SetSimulationTimeMultiplier(uint16_t multiplier)
{
	simulationTimeMultiplier = multiplier;
}
