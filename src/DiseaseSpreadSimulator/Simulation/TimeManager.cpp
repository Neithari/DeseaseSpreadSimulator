#include "Simulation/TimeManager.h"

void DiseaseSpreadSimulation::TimeManager::Update()
{
	simulationTime++;
	dayTime++;

	if (dayTime == hoursPerDay)
	{
		dayTime = 0U;
		currentDay = GetNextDay();

		isWorkday = !(currentDay == Day::Saturday || currentDay == Day::Sunday);
	}
}

uint64_t DiseaseSpreadSimulation::TimeManager::GetElapsedHours() const
{
	return simulationTime;
}

uint64_t DiseaseSpreadSimulation::TimeManager::GetElapsedDays() const
{
	return simulationTime / hoursPerDay;
}

DiseaseSpreadSimulation::Day DiseaseSpreadSimulation::TimeManager::GetCurrentDay() const
{
	return currentDay;
}

uint32_t DiseaseSpreadSimulation::TimeManager::GetTime() const
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
