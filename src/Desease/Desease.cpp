#include "pch.h"
#include "Desease/Desease.h"

DeseaseSpreadSimulation::Desease::Desease(std::string name, const unsigned int id, const int incubationPeriod, const int daysInfectious, std::pair<int, int> deseaseDurationRange, std::vector<float> mortalityByAge, std::pair<int, int> daysTillDeathRange)
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

float DeseaseSpreadSimulation::Desease::GetMortalityByAge(int age) const
{
	// any number devided by 10 will result the tens and we switch on them to get the Age_Group
	// everything above 7 is Age_Group::AboveEighty
	switch (age / 10)
	{
	case 0:
		return GetMortalityByAgeGroup(Age_Group::UnderTen);
		break;
	case 1:
		return GetMortalityByAgeGroup(Age_Group::UnderTwenty);
		break;
	case 2:
		return GetMortalityByAgeGroup(Age_Group::UnderThirty);
		break;
	case 3:
		return GetMortalityByAgeGroup(Age_Group::UnderFourty);
		break;
	case 4:
		return GetMortalityByAgeGroup(Age_Group::UnderFifty);
		break;
	case 5:
		return GetMortalityByAgeGroup(Age_Group::UnderSixty);
		break;
	case 6:
		return GetMortalityByAgeGroup(Age_Group::UnderSeventy);
		break;
	case 7:
		return GetMortalityByAgeGroup(Age_Group::UnderEighty);
		break;
	default:
		return GetMortalityByAgeGroup(Age_Group::AboveEighty);
		break;
	}
}


float DeseaseSpreadSimulation::Desease::GetMortalityByAgeGroup(Age_Group age) const
{
	return mortalityByAge.at(static_cast<size_t>(age));
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

bool DeseaseSpreadSimulation::Desease::isFatal(DeseaseSpreadSimulation::Age_Group age) const
{
	std::random_device seed;
	std::mt19937 randomNumberGenerator(seed());
	std::uniform_real_distribution<float> fatalityDistribution(0.0f, 1.0f);

	// infection is fatal if the random number is <= than the mortality rate
	return (fatalityDistribution(randomNumberGenerator) <= GetMortalityByAgeGroup(age));
}
