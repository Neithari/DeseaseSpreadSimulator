#include "pch.h"
#include "Infection.h"

void DiseaseSpreadSimulation::Infection::Contaminate(const Disease* infection, Age_Group age)
{
	disease = infection;
	seirState = Seir_State::Exposed;

	latentPeriod = disease->IncubationPeriod();
	daysInfectious = disease->DaysInfectious();
	daysTillCured = disease->GetDiseaseDuration();

	spreadFactor = disease->GetSpreadFactor();

	if (disease->isFatal(age))
	{
		isFatal = true;
		daysToLive = disease->DaysTillDeath();
	}
}

const DiseaseSpreadSimulation::Disease* DiseaseSpreadSimulation::Infection::GetDisease()
{
	return disease;
}

void DiseaseSpreadSimulation::Infection::Update(Person& person, bool isNewDay)
{
	if (HasDisease() && isNewDay)
	{
		AdvanceDay(person);
		DiseaseCheck();
	}
}

void DiseaseSpreadSimulation::Infection::AdvanceDay(Person& person)
{
	// If the person has no disease, has recovered, is immune or dead do nothing (recovered/immune/dead are all Seir_State::Recovered)
	if (seirState == Seir_State::Susceptible || seirState == Seir_State::Recovered)
	{
		return;
	}
	if (latentPeriod > 0)
	{
		latentPeriod--;
	}
	if (daysInfectious > 0)
	{
		daysInfectious--;
	}
	if (daysTillCured > 0)
	{
		daysTillCured--;
	}
	if (isFatal)
	{
		// Decrement daysToLive and if it reached 0 the person will die
		if (--daysToLive <= 0)
		{
			person.Kill();
		}
	}
}

bool DiseaseSpreadSimulation::Infection::WillInfect(const Infection& exposed, float acceptanceFactor, const Community* community) const
{
	// Map the acceptance factor to the inverse of the disease spread factor
	// Acceptance factor range is always 0 to 1
	// Disease spread factor range is spreadFactor to 1/10th of spreadFactor
	float probability = Random::MapOneRangeToAnother(acceptanceFactor, 0.f, 1.f, exposed.spreadFactor, exposed.spreadFactor * 0.1f);

	// Decrease probability when there is a mask mandate. Take the median effectiveness of the 3 different masks
	// https://www.cdc.gov/mmwr/volumes/71/wr/mm7106e1.htm
	if (community->ContainmentMeasures().IsMaskMandate())
	{
		probability *= .68333f;
	}

	std::bernoulli_distribution distribution(probability);

	return distribution(Random::generator);
}

bool DiseaseSpreadSimulation::Infection::IsSusceptible() const
{
	return seirState == Seir_State::Susceptible;
}

bool DiseaseSpreadSimulation::Infection::IsInfectious() const
{
	return seirState == Seir_State::Infectious;
}

bool DiseaseSpreadSimulation::Infection::IsFatal() const
{
	return isFatal;
}

bool DiseaseSpreadSimulation::Infection::HasDisease() const
{
	return disease != nullptr;
}

std::string DiseaseSpreadSimulation::Infection::GetDiseaseName() const
{
	if (HasDisease())
	{
		return disease->GetDiseaseName();
	}
	return "";
}

bool DiseaseSpreadSimulation::Infection::HasRecovered() const
{
	return hasRecovered;
}

bool DiseaseSpreadSimulation::Infection::HasSymptoms() const
{
	return hasSymptoms;
}

uint32_t DiseaseSpreadSimulation::Infection::GetSpreadCount() const
{
	return spreadCount;
}

const DiseaseSpreadSimulation::Disease* DiseaseSpreadSimulation::Infection::GetDisease() const
{
	return disease;
}

void DiseaseSpreadSimulation::Infection::DiseaseCheck()
{
	switch (seirState)
	{
	case DiseaseSpreadSimulation::Seir_State::Susceptible:
		break;
	case DiseaseSpreadSimulation::Seir_State::Exposed:
		// Person is infectious when it was exposed to a disease and latent period is over
		if (latentPeriod <= 0)
		{
			hasSymptoms = disease->willDevelopSymptoms();
			seirState = Seir_State::Infectious;
		}
		break;
	case DiseaseSpreadSimulation::Seir_State::Infectious:
		// We switch to recovered state after we stop being infectious but we wait with flagging us recovered
		if (daysInfectious <= 0)
		{
			seirState = Seir_State::Recovered;
		}
		break;
	case DiseaseSpreadSimulation::Seir_State::Recovered:
		if (daysTillCured == 0)
		{
			// Person is recovered when daysTillCured reached 0
			hasRecovered = true;
			hasSymptoms = false;
			disease = nullptr;
		}
		// TODO: Implement that a person can be susceptible again.
		break;
	default:
		break;
	}
}

void DiseaseSpreadSimulation::Infection::IncreaseSpreadCount()
{
	spreadCount++;
}
