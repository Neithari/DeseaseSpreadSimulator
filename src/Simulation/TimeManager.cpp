#include "pch.h"
#include "Simulation/TimeManager.h"

void DeseaseSpreadSimulation::TimeManager::Update()
{
	simulationTime++;
	dayTime++;
	if (dayTime == 24)
	{
		dayTime = 0;
		currentDay = GetNextDay();

		if (currentDay == Day::Saturday || currentDay == Day::Sunday)
		{
			isWorkday = false;
		}
		else
		{
			isWorkday = true;
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
	return isWorkday;
}

DeseaseSpreadSimulation::Day DeseaseSpreadSimulation::TimeManager::GetNextDay() const
{
	if (currentDay == Day::Sunday)
	{
		return Day::Monday;
	}
	return static_cast<Day>(static_cast<int>(currentDay) + 1);
}
