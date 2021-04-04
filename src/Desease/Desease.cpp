#include "pch.h"
#include "Desease\Desease.h"

DeseaseSpreadSimulation::Desease::Desease(std::string name, const int id, const int incubationPeriod, const int daysInfectious, std::pair<int, int> deseaseDurationRange, std::vector<float> mortalityByAge, std::pair<int, int> daysTillDeathRange)
	:
	name(std::move(name)),
	id(id),
	incubationPeriod(incubationPeriod),
	daysInfectious(daysInfectious),
	durationRange(std::move(deseaseDurationRange)),
	mortalityByAge(std::move(mortalityByAge)),
	daysTillDeathRange(std::move(daysTillDeathRange))
{
}

const std::string& DeseaseSpreadSimulation::Desease::GetDeseaseName() const
{
	return name;
}

int DeseaseSpreadSimulation::Desease::IncubationPeriod() const
{
	return incubationPeriod;
}

int DeseaseSpreadSimulation::Desease::DaysInfectious() const
{
	return daysInfectious;
}

float DeseaseSpreadSimulation::Desease::GetMortalityByAge(const unsigned int age) const
{
	if (age < 10)
	{
		return mortalityByAge.at(0);
	}
	if (age < 20)
	{
		return mortalityByAge.at(1);
	}
	if (age < 30)
	{
		return mortalityByAge.at(2);
	}
	if (age < 40)
	{
		return mortalityByAge.at(3);
	}
	if (age < 50)
	{
		return mortalityByAge.at(4);
	}
	if (age < 60)
	{
		return mortalityByAge.at(5);
	}
	if (age < 70)
	{
		return mortalityByAge.at(6);
	}
	if (age < 80)
	{
		return mortalityByAge.at(7);
	}
	
	return mortalityByAge.at(8);
}

int DeseaseSpreadSimulation::Desease::GetDeseaseDuration() const
{
	std::random_device seed;
	std::mt19937 randomNumberGenerator(seed());
	std::uniform_int_distribution<int> deseaseDistribution(durationRange.first, durationRange.second);

	// return a random duration between min and max
	return deseaseDistribution(randomNumberGenerator);
}

int DeseaseSpreadSimulation::Desease::DaysTillDeath() const
{
	std::random_device seed;
	std::mt19937 randomNumberGenerator(seed());
	std::uniform_int_distribution<int> deathDistribution(daysTillDeathRange.first, daysTillDeathRange.second);

	// return a random death time out of the range
	return deathDistribution(randomNumberGenerator);
}

unsigned int DeseaseSpreadSimulation::Desease::GetID() const
{
	return id;
}

bool DeseaseSpreadSimulation::Desease::isSame(const Desease& other) const
{

	bool same = true;

	if (name != other.name) same = false;
	if (id != other.id) same = false;
	if (incubationPeriod != other.incubationPeriod) same = false;
	if (daysInfectious != other.daysInfectious) same = false;
	if (durationRange != other.durationRange) same = false;
	if (!(mortalityByAge == other.mortalityByAge)) same = false;
	if (daysTillDeathRange != other.daysTillDeathRange) same = false;
	return same;
}

bool DeseaseSpreadSimulation::Desease::hasSameID(const Desease& other) const
{
	return GetID() == other.GetID();
}

bool DeseaseSpreadSimulation::Desease::isFatal(const unsigned int age) const
{
	std::random_device seed;
	std::mt19937 randomNumberGenerator(seed());
	std::uniform_real_distribution<float> fatalityDistribution(0.0f, 1.0f);

	// infection is fatal if the random number is <= than the mortality rate
	return (fatalityDistribution(randomNumberGenerator) <= GetMortalityByAge(age));
}
