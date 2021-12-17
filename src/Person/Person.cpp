#include "pch.h"
#include "Person/Person.h"
#include "Person/PersonBehavior.h"
#include "IDGenerator/IDGenerator.h"

DeseaseSpreadSimulation::Person::Person(Age_Group age, Sex sex, PersonBehavior behavior, const Community& community, Home* home)
	:
	id(IDGenerator::IDGenerator<Person>::GetNextID()),
	age(age),
	sex(sex),
	behavior(behavior),
	community(community),
	home(home),
	whereabouts(home)
{
}

void DeseaseSpreadSimulation::Person::Update()
{
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
	
	latentPeriod = desease->IncubationPeriod();
	daysInfectious = desease->DaysInfectious();
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

uint32_t DeseaseSpreadSimulation::Person::GetID() const
{
	return id;
}

DeseaseSpreadSimulation::Age_Group DeseaseSpreadSimulation::Person::GetAgeGroup() const
{
	return age;
}

DeseaseSpreadSimulation::Sex DeseaseSpreadSimulation::Person::GetSex() const
{
	return sex;
}

const DeseaseSpreadSimulation::PersonBehavior& DeseaseSpreadSimulation::Person::GetBehavior() const
{
	return behavior;
}

const DeseaseSpreadSimulation::Community& DeseaseSpreadSimulation::Person::GetCommunity() const
{
	return community;
}

const DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Person::GetWhereabouts() const
{
	return whereabouts;
}

const DeseaseSpreadSimulation::Home* DeseaseSpreadSimulation::Person::GetHome() const
{
	return home;
}

const DeseaseSpreadSimulation::Workplace* DeseaseSpreadSimulation::Person::GetWorkplace() const
{
	return workplace;
}

const DeseaseSpreadSimulation::School* DeseaseSpreadSimulation::Person::GetSchool() const
{
	return school;
}

void DeseaseSpreadSimulation::Person::DeseaseCheck()
{
	// A person is infectious when it was exposed to a desease and 
	if (state == Seir_State::Exposed && latentPeriod <= 0)
	{
		state = Seir_State::Infectious;
	}
	// it is recovered when it is 
	else if (state == Seir_State::Infectious && daysInfectious <= 0)
	{
		state = Seir_State::Recovered;
	}
	else if (state == Seir_State::Recovered && daysTillCured == 0)
	{
		desease = nullptr;
	}
}

void DeseaseSpreadSimulation::Person::SetWhereabouts(const Place* newWhereabouts)
{
	whereabouts = whereabouts;
}

void DeseaseSpreadSimulation::Person::SetWorkplace(const Workplace* newWorkplace)
{
	workplace = newWorkplace;
}

void DeseaseSpreadSimulation::Person::SetHome(Home* newHome)
{
	home = newHome;
	// Check if the person is already somewhere.
	if (!whereabouts)
	{
		// If not set it's whereabouts to home...
		whereabouts = home;
		// ...and put the person in it's home
		home->AddPerson(this);
	}
}

void DeseaseSpreadSimulation::Person::ChangeBehavior(PersonBehavior newBehavior)
{
	behavior = newBehavior;
}

void DeseaseSpreadSimulation::Person::Move(Place* destination)
{
	whereabouts = destination;
}

void DeseaseSpreadSimulation::Person::AdvanceDay()
{
	// If the person has no desease, has recovered, is immune or dead do nothing (recovered/immune/dead are all Seir_State::Recovered)
	if (state == Seir_State::Susceptible || state == Seir_State::Recovered)
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
	if (willDie)
	{
		// decrement daysToLive and if it reached 0 the person will die
		if (--daysToLive <= 0)
		{
			alive = false;
		}
	}
}
