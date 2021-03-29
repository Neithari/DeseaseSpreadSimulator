#include "pch.h"
#include "Desease\Desease.h"

Desease::Desease(const std::string name, const int incubationPeriod, const int daysContagious, const std::pair<int, int> deseaseDurationRange, const std::vector<float>& mortalityByAge, const std::pair<int, int> daysTillDeathRange)
	:
	name(name),
	incubationPeriod(incubationPeriod),
	daysContagious(daysContagious),
	durationRange(deseaseDurationRange),
	mortalityByAge(mortalityByAge),
	daysTillDeathRange(daysTillDeathRange)
{
}

const std::string& Desease::GetDeseaseName() const
{
	return name;
}

const int Desease::IncubationPeriod() const
{
	return incubationPeriod;
}

const int Desease::DaysContagious() const
{
	return daysContagious;
}

const float Desease::GetMortalityByAge(const unsigned int age) const
{
	if (age < 10)
	{
		return mortalityByAge.at(0);
	}
	else if (age < 20)
	{
		return mortalityByAge.at(1);
	}
	else if (age < 30)
	{
		return mortalityByAge.at(2);
	}
	else if (age < 40)
	{
		return mortalityByAge.at(3);
	}
	else if (age < 50)
	{
		return mortalityByAge.at(4);
	}
	else if (age < 60)
	{
		return mortalityByAge.at(5);
	}
	else if (age < 70)
	{
		return mortalityByAge.at(6);
	}
	else if (age < 80)
	{
		return mortalityByAge.at(7);
	}
	else
	{
		return mortalityByAge.at(8);
	}
}

const int Desease::GetDeseaseDuration() const
{
	std::random_device seed;
	std::mt19937 randomNumberGenerator(seed());
	std::uniform_int_distribution<int> deseaseDistribution(durationRange.first, durationRange.second);

	// return a random duration between min and max
	return deseaseDistribution(randomNumberGenerator);
}

const int Desease::DaysTillDeath() const
{
	std::random_device seed;
	std::mt19937 randomNumberGenerator(seed());
	std::uniform_int_distribution<int> deathDistribution(daysTillDeathRange.first, daysTillDeathRange.second);

	// return a random death time out of the range
	return deathDistribution(randomNumberGenerator);
}

const bool Desease::isSame(Desease other) const
{
	bool same = true;

	if (name != other.name)
		same = false;
	if (incubationPeriod != other.incubationPeriod)
		same = false;
	if (daysContagious != other.daysContagious)
		same = false;
	if (durationRange != other.durationRange)
		same = false;
	if (!(mortalityByAge == other.mortalityByAge))
		same = false;
	if (daysTillDeathRange != other.daysTillDeathRange)
		same = false;
	
	return same;
}

const bool Desease::isFatal(const unsigned int age) const
{
	std::random_device seed;
	std::mt19937 randomNumberGenerator(seed());
	std::uniform_real_distribution<float> fatalityDistribution(0.0f, 1.0f);

	// infection is fatal if the random number is <= than the mortality rate
	return (fatalityDistribution(randomNumberGenerator) <= GetMortalityByAge(age));
}
