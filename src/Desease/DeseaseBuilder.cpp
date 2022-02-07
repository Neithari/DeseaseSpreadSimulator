#include "pch.h"
#include "Desease/DeseaseBuilder.h"

DeseaseSpreadSimulation::Desease DeseaseSpreadSimulation::DeseaseBuilder::CreateCorona()
{
	SetDeseaseName("COVID-19");
	SetIncubationPeriod(6);
	SetDaysInfectious(8);
	SetDeseaseDuration(14, 42);
	SetMortalityByAge({ 0.0f, 0.0014f, 0.0012f, 0.002f, 0.0038f, 0.0098f, .0298f, .0794f, .1734f });
	SetDaysTillDeath(14, 56);

	return CreateDesease();
}

DeseaseSpreadSimulation::Desease DeseaseSpreadSimulation::DeseaseBuilder::CreateDeadlyTestDesease()
{
	SetDeseaseName("DeadlyTestDesease");
	SetIncubationPeriod(1);
	SetDaysInfectious(10);
	SetDeseaseDuration(10, 10);
	SetMortalityByAge({ 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f });
	SetDaysTillDeath(10, 10);

	return CreateDesease();
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetDeseaseName(std::string deseaseName)
{
	setupDone[0] = true;

	name = std::move(deseaseName);
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetIncubationPeriod(const uint16_t period)
{
	setupDone[1] = true;

	incubationPeriod = period;
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetDaysInfectious(const uint16_t days)
{
	setupDone[2] = true;

	daysInfectious = days;
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetDeseaseDuration(const uint16_t min, const uint16_t max)
{
	setupDone[3] = true;

	deseaseDurationRange = { min, max };
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetMortalityByAge(std::vector<float> mortality)
{
	setupDone[4] = true;

	mortalityByAge = std::move(mortality);
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetDaysTillDeath(const uint16_t min, const uint16_t max)
{
	setupDone[5] = true;

	daysTillDeathRange = { min, max };
}

DeseaseSpreadSimulation::Desease DeseaseSpreadSimulation::DeseaseBuilder::CreateDesease()
{
	for (bool& setup : setupDone)
	{
		// will throw if you didn't setup everything befor trying to create the desease
		if (!setup)
		{
			throw std::logic_error("Complete setup to create a desease!");
		}
	}

	return Desease(name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange);
}
