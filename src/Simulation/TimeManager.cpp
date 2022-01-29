#include "pch.h"
#include "Simulation/TimeManager.h"

DeseaseSpreadSimulation::TimeManager::TimeManager()
	:
	lastTime(std::chrono::steady_clock::now())
{
}

DeseaseSpreadSimulation::TimeManager& DeseaseSpreadSimulation::TimeManager::Instance()
{
	static TimeManager instance;
	
	return instance;
}

void DeseaseSpreadSimulation::TimeManager::Start()
{
	lastTime = std::chrono::steady_clock::now();
	pauseTime = false;
}

void DeseaseSpreadSimulation::TimeManager::Pause()
{
	pauseTime = true;
}

void DeseaseSpreadSimulation::TimeManager::Update()
{
	if (!pauseTime)
	{
		// Update the frame times...
		auto currentTime = std::chrono::steady_clock::now();
		// ...save the duration of the last frame
		auto frameTime = currentTime - lastTime;
		lastTime = currentTime;

		// Scale the frame time with the multiplier and sum it
		frameSum += frameTime * simulationTimeMultiplier;

		// Advance the simulation time in fixed time steps
		while (frameSum >= timeStep)
		{
			simulationTime++;
			dayTime++;
			if (dayTime >= 24)
			{
				dayTime = 0 + dayTime - 24;
				currentDay = GetNextDay();
				NotifyDayChange();
			}
			frameSum -= timeStep;
		}
	}
}

uint64_t DeseaseSpreadSimulation::TimeManager::GetElapsedHours() const
{
	return simulationTime;
}

uint64_t DeseaseSpreadSimulation::TimeManager::GetElapsedDays() const
{
	return simulationTime / 24u;
}

void DeseaseSpreadSimulation::TimeManager::SetSimulationTimeMultiplier(uint16_t multiplier)
{
	simulationTimeMultiplier = multiplier;
}

DeseaseSpreadSimulation::Day DeseaseSpreadSimulation::TimeManager::GetCurrentDay() const
{
	return currentDay;
}

uint16_t DeseaseSpreadSimulation::TimeManager::GetTime() const
{
	return dayTime;
}

bool DeseaseSpreadSimulation::TimeManager::IsWorkday() const
{
	return IsWorkday(currentDay);
}

bool DeseaseSpreadSimulation::TimeManager::IsWorkday(const Day day)
{
	return !(day == Day::Saturday || day == Day::Sunday);
}

void DeseaseSpreadSimulation::TimeManager::AddObserver(TimeObserver* observer)
{
	std::lock_guard<std::mutex> lockGuard(observersMutex);
	observers.push_back(observer);
}

void DeseaseSpreadSimulation::TimeManager::RemoveObserver(TimeObserver* observer)
{
	std::lock_guard<std::mutex> lockGuard(observersMutex);
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
		observer->OnNewDay(currentDay);
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
