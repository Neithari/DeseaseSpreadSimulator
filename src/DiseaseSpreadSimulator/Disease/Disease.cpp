#include "Disease/Disease.h"
#include "IDGenerator/IDGenerator.h"
#include "RandomNumbers.h"

DiseaseSpreadSimulation::Disease::Disease(std::string name,
	std::pair<uint32_t, uint32_t> incubationPeriod,
	uint32_t daysInfectious,
	std::pair<uint32_t, uint32_t> diseaseDurationRange,
	std::vector<float> mortalityByAge,
	std::pair<uint32_t, uint32_t> daysTillDeathRange,
	std::pair<float, float> spreadFactor,
	float testAccuracy,
	std::pair<float, float> symptomsDevelopment)
	: m_id(IDGenerator::IDGenerator<Disease>::GetNextID()),
	  m_name(std::move(name)),
	  m_incubationPeriod(std::move(incubationPeriod)),
	  m_daysInfectious(daysInfectious),
	  m_durationRange(std::move(diseaseDurationRange)),
	  m_mortalityByAge(std::move(mortalityByAge)),
	  m_daysTillDeathRange(std::move(daysTillDeathRange)),
	  m_spreadFactor(std::move(spreadFactor)),
	  m_testAccuracy(testAccuracy),
	  m_symptomsDevelopment(std::move(symptomsDevelopment))
{
}

const std::string& DiseaseSpreadSimulation::Disease::GetDiseaseName() const
{
	return m_name;
}

uint32_t DiseaseSpreadSimulation::Disease::IncubationPeriod() const
{
	/// TODO: Implement a lognormal distribution for random number generation

	return Random::UniformIntRange(m_incubationPeriod.first, m_incubationPeriod.second);
}

uint32_t DiseaseSpreadSimulation::Disease::DaysInfectious() const
{
	return m_daysInfectious;
}

// Silence clang tidy because of false positive cases 5-7 are markt
// NOLINTBEGIN(*-magic-numbers)
float DiseaseSpreadSimulation::Disease::GetMortalityByAge(uint32_t age) const
{
	// Any number devided by 10 will result the tens and we switch on them to get the Age_Group
	// Everything above 7 is Age_Group::AboveEighty
	static constexpr uint32_t tenths{10};
	switch (age / tenths)
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
// NOLINTEND(*-magic-numbers)

float DiseaseSpreadSimulation::Disease::GetMortalityByAgeGroup(Age_Group age) const
{
	return m_mortalityByAge.at(static_cast<size_t>(age));
}

uint32_t DiseaseSpreadSimulation::Disease::GetDiseaseDuration() const
{
	return Random::UniformIntRange(m_durationRange.first, m_durationRange.second);
}

uint32_t DiseaseSpreadSimulation::Disease::DaysTillDeath() const
{
	return Random::UniformIntRange(m_daysTillDeathRange.first, m_daysTillDeathRange.second);
}

float DiseaseSpreadSimulation::Disease::GetSpreadFactor() const
{
	/// TODO: Implement a lognormal distribution for random number generation

	return Random::MapRangeToPercent(Random::UniformFloatRange(m_spreadFactor.first, m_spreadFactor.second), m_spreadFactor.first, m_spreadFactor.second);
}

float DiseaseSpreadSimulation::Disease::GetTestAccuracy() const
{
	return m_testAccuracy;
}

uint32_t DiseaseSpreadSimulation::Disease::GetID() const
{
	return m_id;
}

bool DiseaseSpreadSimulation::Disease::isSame(const Disease& other) const
{
	return m_name == other.m_name
		   && m_incubationPeriod == other.m_incubationPeriod
		   && m_daysInfectious == other.m_daysInfectious
		   && m_durationRange == other.m_durationRange
		   && m_mortalityByAge == other.m_mortalityByAge
		   && m_daysTillDeathRange == other.m_daysTillDeathRange
		   && m_spreadFactor == other.m_spreadFactor
		   && m_testAccuracy == other.m_testAccuracy
		   && m_symptomsDevelopment == other.m_symptomsDevelopment;
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
	return Random::Percent<float>() <= Random::UniformFloatRange(m_symptomsDevelopment.first, m_symptomsDevelopment.second);
}
