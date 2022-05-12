#include "pch.h"
#include "Simulation/TimeManager.h"

void DiseaseSpreadSimulation::TimeManager::Update()
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

uint64_t DiseaseSpreadSimulation::TimeManager::GetElapsedHours() const
{
	return simulationTime;
}

uint64_t DiseaseSpreadSimulation::TimeManager::GetElapsedDays() const
{
	return simulationTime / 24u;
}

DiseaseSpreadSimulation::Day DiseaseSpreadSimulation::TimeManager::GetCurrentDay() const
{
	return currentDay;
}

uint16_t DiseaseSpreadSimulation::TimeManager::GetTime() const
{
	return dayTime;
}

bool DiseaseSpreadSimulation::TimeManager::IsWorkday() const
{
	return isWorkday;
}

DiseaseSpreadSimulation::Day DiseaseSpreadSimulation::TimeManager::GetNextDay() const
{
	if (currentDay == Day::Sunday)
	{
		return Day::Monday;
	}
	return static_cast<Day>(static_cast<int>(currentDay) + 1);
}
