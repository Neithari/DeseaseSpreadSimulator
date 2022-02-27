#include "pch.h"
#include "Infection.h"

void DeseaseSpreadSimulation::Infection::Contaminate(const Desease* infection, Age_Group age)
{
	desease = infection;
	seirState = Seir_State::Exposed;

	latentPeriod = desease->IncubationPeriod();
	daysInfectious = desease->DaysInfectious();
	daysTillCured = desease->GetDeseaseDuration();

	hasSymptoms = desease->willDevelopSymptoms();

	if (desease->isFatal(age))
	{
		isFatal = true;
		daysToLive = desease->DaysTillDeath();
	}
}

const DeseaseSpreadSimulation::Desease* DeseaseSpreadSimulation::Infection::GetDesease()
{
	return desease;
}

void DeseaseSpreadSimulation::Infection::Update(Person& person, bool isNewDay)
{
	if (HasDesease() && isNewDay)
	{
		AdvanceDay(person);
		DeseaseCheck();
	}
}

void DeseaseSpreadSimulation::Infection::AdvanceDay(Person& person)
{
	// If the person has no desease, has recovered, is immune or dead do nothing (recovered/immune/dead are all Seir_State::Recovered)
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

bool DeseaseSpreadSimulation::Infection::WillInfect(const Desease* exposed, float acceptanceFactor) const
{
	// Map the acceptance factor to the inverse of the desease spread factor
	// Acceptance factor range is always 0 to 1
	// Desease spread factor range is spreadFactor to 1/10th of spreadFactor
	float probability = Random::MapOneRangeToAnother(acceptanceFactor, 0.f, 1.f, exposed->GetSpreadFactor(), exposed->GetSpreadFactor() * 0.1f);

	std::bernoulli_distribution distribution(probability);

	return distribution(Random::generator);
}

bool DeseaseSpreadSimulation::Infection::IsSusceptible() const
{
	return seirState == Seir_State::Susceptible;
}

bool DeseaseSpreadSimulation::Infection::IsInfectious() const
{
	return seirState == Seir_State::Infectious;
}

bool DeseaseSpreadSimulation::Infection::IsFatal() const
{
	return isFatal;
}

bool DeseaseSpreadSimulation::Infection::HasDesease() const
{
	return desease != nullptr;
}

std::string DeseaseSpreadSimulation::Infection::GetDeseaseName() const
{
	if (HasDesease())
	{
		return desease->GetDeseaseName();
	}
	return "";
}

bool DeseaseSpreadSimulation::Infection::HasRecovered() const
{
	return hasRecovered;
}

bool DeseaseSpreadSimulation::Infection::HasSymptoms() const
{
	return hasSymptoms;
}

uint32_t DeseaseSpreadSimulation::Infection::GetSpreadCount() const
{
	return spreadCount;
}

const DeseaseSpreadSimulation::Desease* DeseaseSpreadSimulation::Infection::GetDesease() const
{
	return desease;
}

void DeseaseSpreadSimulation::Infection::DeseaseCheck()
{
	switch (seirState)
	{
	case DeseaseSpreadSimulation::Seir_State::Susceptible:
		break;
	case DeseaseSpreadSimulation::Seir_State::Exposed:
		// Person is infectious when it was exposed to a desease and latent period is over
		if (latentPeriod <= 0)
		{
			seirState = Seir_State::Infectious;
		}
		break;
	case DeseaseSpreadSimulation::Seir_State::Infectious:
		// Person is recovered when daysInfectious reached 0
		if (daysInfectious <= 0)
		{
			seirState = Seir_State::Recovered;
			hasRecovered = true;
		}
		break;
	case DeseaseSpreadSimulation::Seir_State::Recovered:
		if (daysTillCured == 0)
		{
			hasSymptoms = false;
			desease = nullptr;
		}
		// TODO: Implement that a person can be susceptible again.
		break;
	default:
		break;
	}
}

void DeseaseSpreadSimulation::Infection::IncreaseSpreadCount()
{
	spreadCount++;
}
