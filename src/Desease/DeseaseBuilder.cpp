#include "pch.h"
#include "Desease/DeseaseBuilder.h"

DeseaseSpreadSimulation::Desease DeseaseSpreadSimulation::DeseaseBuilder::CreateCorona()
{
	// https://www.rki.de/DE/Content/InfAZ/N/Neuartiges_Coronavirus/Steckbrief.html
	SetDeseaseName("COVID-19");
	static constexpr uint16_t coronaIncubationPeriod{ 6 };
	SetIncubationPeriod(coronaIncubationPeriod);
	SetDaysInfectious(10);
	static constexpr uint16_t coronaDurationMin{ 8 };
	static constexpr uint16_t coronaDurationMax{ 10 };
	SetDeseaseDuration(coronaIncubationPeriod + coronaDurationMin, coronaIncubationPeriod + coronaDurationMax);
	SetMortalityByAge({ 0.0f, 0.0014f, 0.0012f, 0.002f, 0.0038f, 0.0098f, .0298f, .0794f, .1734f });
	SetDaysTillDeath(14, 56);

	// According to multiple sources only few individuals infect a lot of people.
	// We use a log normal distribution with most people in the low percents because of that.
	std::lognormal_distribution factorDistribution(0.0f,0.5f);
	SetSpreadFactor(Random::MapRangeToPercent(factorDistribution(Random::generator), factorDistribution.min(), factorDistribution.max()));

	SetTestAccuracy(0.981f);
	// https://www.rki.de/DE/Content/InfAZ/N/Neuartiges_Coronavirus/Steckbrief.html -> Manifestationsindex
	SetSymptomsDevelopment(0.55f, 0.85f);

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
	SetSpreadFactor(1.0f);
	SetTestAccuracy(1.0f);
	SetSymptomsDevelopment(1.f, 1.f);

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

void DeseaseSpreadSimulation::DeseaseBuilder::SetSpreadFactor(const float factor)
{
	setupDone[6] = true;

	spreadFactor = factor;
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetTestAccuracy(const float accuracy)
{
	setupDone[7] = true;

	testAccuracy = accuracy;
}

void DeseaseSpreadSimulation::DeseaseBuilder::SetSymptomsDevelopment(const float minPercent, const float maxPercent)
{
	setupDone[8] = true;

	symptomsDevelopment.first = minPercent;
	symptomsDevelopment.second = maxPercent;
}

DeseaseSpreadSimulation::Desease DeseaseSpreadSimulation::DeseaseBuilder::CreateDesease()
{
	for (bool& setup : setupDone)
	{
		// Will throw if you didn't setup everything befor trying to create the desease
		if (!setup)
		{
			throw std::logic_error("Complete setup to create a desease!");
		}
	}

	return Desease{ name,
		incubationPeriod,
		daysInfectious,
		deseaseDurationRange,
		mortalityByAge,
		daysTillDeathRange,
		spreadFactor,
		testAccuracy,
		symptomsDevelopment };
}
