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
	// while the sum > tick advance the simulationTime
	while (frameSum >= tick)
	{
		simulationTime++;
		currentDay = GetNextDay();
		NotifyDayChange();
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

DeseaseSpreadSimulation::Day DeseaseSpreadSimulation::TimeManager::GetCurrentDay() const
{
	return currentDay;
}

void DeseaseSpreadSimulation::TimeManager::AddObserver(TimeObserver* observer)
{
	observers.push_back(observer);
}

void DeseaseSpreadSimulation::TimeManager::RemoveObserver(TimeObserver* observer)
{
	auto itToRemove = std::find(observers.begin(), observers.end(), observer);
	if (itToRemove != observers.end())
	{
		observers.erase(itToRemove);
	}
}

void DeseaseSpreadSimulation::TimeManager::NotifyDayChange()
{
	for (auto observer : observers)
	{
		observer->OnNewDay();
	}
}

DeseaseSpreadSimulation::Day DeseaseSpreadSimulation::TimeManager::GetNextDay() const
{
	if (currentDay == Day::Sunday)
	{
		return Day::Monday;
	}
	return static_cast<Day>(static_cast<int>(currentDay) + 1);
}
