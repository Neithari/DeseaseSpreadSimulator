#include "pch.h"
#include "Person\Person.h"

DeseaseSpreadSimulation::Person::Person(unsigned int age, std::pair<float, float> position)
	:
	age(age),
	position(std::move(position))
{
}

void DeseaseSpreadSimulation::Person::Update()
{
	// move person
	Move();
	//  if we have a desease...
	if (desease != nullptr)
	{
		// ...advance day
		AdvanceDay();
		// ...desease check
		DeseaseCheck();
	}
}

void DeseaseSpreadSimulation::Person::Contact(Person& other)
{
	/// TODO: Should be possible to only alter myself and take other as const

	// if the other person is infectious and I have no desease, now I have
	if (other.isInfectious() && isSusceptible())
	{
		Contaminate(other.desease);
		other.spreadCount++;
	}
	// if I am infectious and the other person has no desease, now he has
	 else if (isInfectious() && other.isSusceptible())
	{
		other.Contaminate(desease);
		spreadCount++;
	}
}

std::string DeseaseSpreadSimulation::Person::GetDeseaseName() const
{
	if (desease != nullptr)
	{
		return desease->GetDeseaseName();
	}
	return "";
}

void DeseaseSpreadSimulation::Person::Contaminate(const Desease* infection)
{
	desease = infection;
	state = Seir_State::Exposed;
	
	daysTillOutbreak = desease->IncubationPeriod();
	daysInfectious = desease->daysInfectious();
	daysTillCured = desease->GetDeseaseDuration();
	// check if the person will die from the infection
	if (desease->isFatal(age))
	{
		willDie = true;
		daysToLive = desease->DaysTillDeath();
	}
}

bool DeseaseSpreadSimulation::Person::isSusceptible() const
{
	return state == Seir_State::Susceptible;
}

bool DeseaseSpreadSimulation::Person::isInfectious() const
{
	return state == Seir_State::Infectious;
}

bool DeseaseSpreadSimulation::Person::isQuarantined() const
{
	return quarantined;
}

bool DeseaseSpreadSimulation::Person::isAlive() const
{
	return alive;
}

bool DeseaseSpreadSimulation::Person::hasDesease(const std::string& deseaseName) const
{
	if (desease == nullptr)
	{
		return false;
	}
	return desease->GetDeseaseName() == deseaseName;
	
}

void DeseaseSpreadSimulation::Person::DeseaseCheck()
{
	if (desease != nullptr)
	{
		if (state == Seir_State::Exposed)
		{
			// if daysInfectious is 0 person had the desease already but is not fully cured
			if (daysTillOutbreak == 0 && daysInfectious > 0)
			{
				// person is infectious when it has a desease and daysTillOutbreak reached 0
				state = Seir_State::Infectious;
			}
		}
		else
		{
			if (daysInfectious == 0)
			{
				state = Seir_State::Recovered;
			}
		}
		if (daysTillCured == 0)
		{
			willDie = false;
			desease = nullptr;
		}
	}
}

void DeseaseSpreadSimulation::Person::Move()
{
	/// TODO: implement move function
}

void DeseaseSpreadSimulation::Person::AdvanceDay()
{
	if (desease != nullptr)
	{
		if (!contagious && daysTillOutbreak > 0)
		{
			daysTillOutbreak--;
		}
		else if (daysInfectious > 0)
		{
			daysInfectious--;
		}
		if (daysTillCured > 0)
		{
			daysTillCured--;
		}
		if (willDie)
		{
			daysToLive--;
		}
	}
}
