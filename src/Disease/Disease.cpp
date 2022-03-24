#include "pch.h"
#include "Disease/Disease.h"
#include "IDGenerator/IDGenerator.h"

DiseaseSpreadSimulation::Disease::Disease(std::string name,
	const std::pair<uint16_t, uint16_t> incubationPeriod,
	const uint16_t daysInfectious,
	std::pair<uint16_t, uint16_t> diseaseDurationRange,
	std::vector<float> mortalityByAge,
	std::pair<uint16_t, uint16_t> daysTillDeathRange,
	std::pair<float, float> spreadFactor,
	float testAccuracy,
	std::pair<float, float> symptomsDevelopment)
	:
	id(IDGenerator::IDGenerator<Disease>::GetNextID()),
	name(std::move(name)),
	incubationPeriod(incubationPeriod),
	daysInfectious(daysInfectious),
	durationRange(std::move(diseaseDurationRange)),
	mortalityByAge(std::move(mortalityByAge)),
	daysTillDeathRange(std::move(daysTillDeathRange)),
	spreadFactor(spreadFactor),
	testAccuracy(testAccuracy),
	symptomsDevelopment(symptomsDevelopment)
{
}

const std::string& DiseaseSpreadSimulation::Disease::GetDiseaseName() const
{
	return name;
}

uint16_t DiseaseSpreadSimulation::Disease::IncubationPeriod() const
{
	/// TODO: Implement a lognormal distribution for random number generation

	return Random::UniformIntRange(incubationPeriod.first, incubationPeriod.second);
}

uint16_t DiseaseSpreadSimulation::Disease::DaysInfectious() const
{
	return daysInfectious;
}

float DiseaseSpreadSimulation::Disease::GetMortalityByAge(uint16_t age) const
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


float DiseaseSpreadSimulation::Disease::GetMortalityByAgeGroup(Age_Group age) const
{
	return mortalityByAge.at(static_cast<size_t>(age));
}

uint16_t DiseaseSpreadSimulation::Disease::GetDiseaseDuration() const
{
	return Random::UniformIntRange(durationRange.first, durationRange.second);
}

uint16_t DiseaseSpreadSimulation::Disease::DaysTillDeath() const
{
	return Random::UniformIntRange(daysTillDeathRange.first, daysTillDeathRange.second);
}

float DiseaseSpreadSimulation::Disease::GetSpreadFactor() const
{
	/// TODO: Implement a lognormal distribution for random number generation

	return Random::MapRangeToPercent(Random::UniformFloatRange(spreadFactor.first, spreadFactor.second), spreadFactor.first, spreadFactor.second);
}

float DiseaseSpreadSimulation::Disease::GetTestAccuracy() const
{
	return testAccuracy;
}

uint32_t DiseaseSpreadSimulation::Disease::GetID() const
{
	return id;
}

bool DiseaseSpreadSimulation::Disease::isSame(const Disease& other) const
{
	return	name == other.name
		 && incubationPeriod == other.incubationPeriod
		 && daysInfectious == other.daysInfectious
		 && durationRange == other.durationRange
		 && mortalityByAge == other.mortalityByAge
		 && daysTillDeathRange == other.daysTillDeathRange
		 && spreadFactor == other.spreadFactor
		 && testAccuracy == other.testAccuracy
		 && symptomsDevelopment == other.symptomsDevelopment;
}

bool DiseaseSpreadSimulation::Disease::hasSameID(const Disease& other) const
{
	return GetID() == other.GetID();
}

bool DiseaseSpreadSimulation::Disease::isFatal(DiseaseSpreadSimulation::Age_Group age) const
{
	// Infection is fatal if the random number is <= than the mortality rate
	return (Random::Percent<float>() <= GetMortalityByAgeGroup(age));
}

bool DiseaseSpreadSimulation::Disease::willDevelopSymptoms() const
{
	// Will return true when the random percent is within the symtoms development percent
	return Random::Percent<float>() <= Random::UniformFloatRange(symptomsDevelopment.first, symptomsDevelopment.second);
}
