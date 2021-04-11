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
	switch (age)
	{
	case Age_Group::UnderTen:
		return mortalityByAge.at(0);
		break;
	case Age_Group::UnderTwenty:
		return mortalityByAge.at(1);
		break;
	case Age_Group::UnderThirty:
		return mortalityByAge.at(2);
		break;
	case Age_Group::UnderFourty:
		return mortalityByAge.at(3);
		break;
	case Age_Group::UnderFifty:
		return mortalityByAge.at(4);
		break;
	case Age_Group::UnderSixty:
		return mortalityByAge.at(5);
		break;
	case Age_Group::UnderSeventy:
		return mortalityByAge.at(6);
		break;
	case Age_Group::UnderEighty:
		return mortalityByAge.at(7);
		break;
	case Age_Group::AboveEighty:
		return mortalityByAge.at(8);
		break;
	default:
		throw std::logic_error("No Valid Age!");
		break;
	}
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
