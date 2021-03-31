#include "pch.h"
#include "Person\Person.h"

Person::Person(unsigned int age, std::pair<float, float> position)
	:
	age(age),
	position(position)
{
}

void Person::Update()
{
	// move person
	Move();
	//  if we have a desease...
	if (desease)
	{
		// ...advance day
		AdvanceDay();
		// ...desease check
		DeseaseCheck();
	}
}

void Person::Contact(Person& other)
{
	// if the other person is contagious and I have no desease, now I have
	if (other.contagious && desease == nullptr)
	{
		Contaminate(other.desease);
		other.spreadCount++;
	}
	// if I am contagious and the other person has no desease, now he has
	if (contagious && other.desease == nullptr)
	{
		other.Contaminate(desease);
		spreadCount++;
	}
}

const std::string Person::GetDeseaseName() const
{
	if (desease)
	{
		return desease->GetDeseaseName();
	}
	return "";
}

void Person::Contaminate(const Desease* infection)
{
	desease = infection;
	
	daysTillOutbreak = desease->IncubationPeriod();
	daysContagious = desease->DaysContagious();
	daysTillCured = desease->GetDeseaseDuration();
	// check if the person will die from the infection
	if (desease->isFatal(age))
	{
		willDie = true;
		daysToLive = desease->DaysTillDeath();
	}
}

const bool Person::isReceptible() const
{
	return receptible;
}

const bool Person::isContagious() const
{
	return contagious;
}

const bool Person::isQuarantined() const
{
	return quarantined;
}

const bool Person::isAlive() const
{
	return alive;
}

const bool Person::hasDesease(const std::string& deseaseName) const
{
	if (!desease)
	{
		return false;
	}
	return desease->GetDeseaseName() == deseaseName;
	
}

void Person::DeseaseCheck()
{
	if (desease)
	{
		if (!contagious)
		{
			// if daysContagious is 0 person had the desease already but is not fully cured
			if (daysTillOutbreak == 0 && daysContagious > 0)
			{
				// person is contagious when it has a desease and daysTillOutbreak reached 0
				contagious = true;
			}
		}
		else
		{
			if (daysContagious == 0)
			{
				contagious = false;
			}
		}
		if (daysTillCured == 0)
		{
			contagious = false;
			willDie = false;
			desease = nullptr;
		}
	}
}

void Person::Move()
{
	/// TODO: implement move function
}

void Person::AdvanceDay()
{
	if (desease)
	{
		if (!contagious && daysTillOutbreak > 0)
		{
			daysTillOutbreak--;
		}
		else if (daysContagious > 0)
		{
			daysContagious--;
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
