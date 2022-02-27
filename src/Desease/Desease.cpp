#include "pch.h"
#include "Desease/Desease.h"
#include "IDGenerator/IDGenerator.h"

DeseaseSpreadSimulation::Desease::Desease(std::string name,
	const uint16_t incubationPeriod,
	const uint16_t daysInfectious,
	std::pair<uint16_t, uint16_t> deseaseDurationRange,
	std::vector<float> mortalityByAge,
	std::pair<uint16_t, uint16_t> daysTillDeathRange,
	float spreadFactor,
	float testAccuracy,
	std::pair<float, float> symptomsDevelopment)
	:
	id(IDGenerator::IDGenerator<Desease>::GetNextID()),
	name(std::move(name)),
	incubationPeriod(incubationPeriod),
	daysInfectious(daysInfectious),
	durationRange(std::move(deseaseDurationRange)),
	mortalityByAge(std::move(mortalityByAge)),
	daysTillDeathRange(std::move(daysTillDeathRange)),
	spreadFactor(spreadFactor),
	testAccuracy(testAccuracy),
	symptomsDevelopment(symptomsDevelopment)
{
}

const std::string& DeseaseSpreadSimulation::Desease::GetDeseaseName() const
{
	return name;
}

uint16_t DeseaseSpreadSimulation::Desease::IncubationPeriod() const
{
	return incubationPeriod;
}

uint16_t DeseaseSpreadSimulation::Desease::DaysInfectious() const
{
	return daysInfectious;
}

float DeseaseSpreadSimulation::Desease::GetMortalityByAge(uint16_t age) const
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

uint16_t DeseaseSpreadSimulation::Desease::GetDeseaseDuration() const
{
	return Random::UniformIntRange(durationRange.first, durationRange.second);
}

uint16_t DeseaseSpreadSimulation::Desease::DaysTillDeath() const
{
	return Random::UniformIntRange(daysTillDeathRange.first, daysTillDeathRange.second);
}

float DeseaseSpreadSimulation::Desease::GetSpreadFactor() const
{
	return spreadFactor;
}

float DeseaseSpreadSimulation::Desease::GetTestAccuracy() const
{
	return testAccuracy;
}

uint32_t DeseaseSpreadSimulation::Desease::GetID() const
{
	return id;
}

bool DeseaseSpreadSimulation::Desease::isSame(const Desease& other) const
{
	return	name == other.name &&
			incubationPeriod == other.incubationPeriod &&
			daysInfectious == other.daysInfectious &&
			durationRange == other.durationRange &&
			mortalityByAge == other.mortalityByAge &&
			daysTillDeathRange == other.daysTillDeathRange;
}

bool DeseaseSpreadSimulation::Desease::hasSameID(const Desease& other) const
{
	return GetID() == other.GetID();
}

bool DeseaseSpreadSimulation::Desease::isFatal(DeseaseSpreadSimulation::Age_Group age) const
{
	// Infection is fatal if the random number is <= than the mortality rate
	return (Random::Percent<float>() <= GetMortalityByAgeGroup(age));
}

bool DeseaseSpreadSimulation::Desease::willDevelopSymptoms() const
{
	// Will return true when the random percent is within the symtoms development percent
	return Random::Percent<float>() <= Random::UniformFloatRange(symptomsDevelopment.first, symptomsDevelopment.second);
}
