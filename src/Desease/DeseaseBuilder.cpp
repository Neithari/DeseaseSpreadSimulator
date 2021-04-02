#include "pch.h"
#include "Desease/DeseaseBuilder.h"

const DeseaseSpreadSimulation::Desease& DeseaseSpreadSimulation::DeseaseBuilder::CreateCorona()
{
	SetDeseaseName("COVID-19");
	SetIncubationPeriod(6);
	SetDaysContagious(8);
	SetDeseaseDuration(14, 42);
	SetMortalityByAge({ 0.0f, 0.14f, 0.12f, 0.2f, 0.38f, 0.98f, 2.98f, 7.94f, 17.34f });
	SetDaysTillDeath(14, 56);

	return CreateDesease();
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetDeseaseName(std::string deseaseName)
{
	setupDone[0] = true;

	name = std::move(deseaseName);
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetIncubationPeriod(const int period)
{
	setupDone[1] = true;

	incubationPeriod = period;
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetDaysContagious(const int days)
{
	setupDone[2] = true;

	daysContagious = days;
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetDeseaseDuration(const int min, const int max)
{
	setupDone[3] = true;

	deseaseDurationRange = { min, max };
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetMortalityByAge(std::vector<float> mortality)
{
	setupDone[4] = true;

	mortalityByAge = std::move(mortality);
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetDaysTillDeath(const int min, const int max)
{
	setupDone[5] = true;

	daysTillDeathRange = { min, max };
}

const DeseaseSpreadSimulation::Desease& DeseaseSpreadSimulation::DeseaseBuilder::CreateDesease()
{
	for (bool& setup : setupDone)
	{
		// will throw if you didn't setup everything befor trying to create the desease
		if (!setup)
		{
			throw std::logic_error("Complete setup to create a desease!");
		}
	}

	return deseases.emplace_back(name, GetID(), incubationPeriod, daysContagious, deseaseDurationRange, mortalityByAge, daysTillDeathRange);
}

unsigned int DeseaseSpreadSimulation::DeseaseBuilder::GetID()
{
	static unsigned int id = 0;
	return id++;
}
