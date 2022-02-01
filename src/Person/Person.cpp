#include "pch.h"
#include "Person/Person.h"
#include "IDGenerator/IDGenerator.h"
#include "Simulation/TimeManager.h"

DeseaseSpreadSimulation::Person::Person(Age_Group age, Sex sex, PersonBehavior behavior, Community* community, Home* home)
	:
	id(IDGenerator::IDGenerator<Person>::GetNextID()),
	age(age),
	sex(sex),
	behavior(behavior),
	community(community),
	home(home),
	whereabouts(home),
	personState(std::make_shared<HomeState>(behavior.foodBuyInterval, behavior.hardwareBuyInterval, TimeManager::Instance().GetCurrentDay()))
{
}

void DeseaseSpreadSimulation::Person::Update()
{
	auto newPersonState = personState->HandleStateChange(*this, TimeManager::Instance().GetTime());
	if (newPersonState)
	{
		personState = std::move(newPersonState);
		personState->Enter(*this);
	}

	if (desease)
	{
		DeseaseCheck();
	}
}

void DeseaseSpreadSimulation::Person::Contact(Person& other)
{
	/// TODO: Should be possible to only alter myself and take other as const

	// if the other person is infectious and I have no desease, now I have
	if (other.isInfectious() && isSusceptible())
	{
		if (WillInfect(other.desease))
		{
			Contaminate(other.desease);
			other.spreadCount++;
		}
	}
	// if I am infectious and the other person has no desease, now he has
	else if (isInfectious() && other.isSusceptible())
	{
		if (other.WillInfect(desease))
		{
			other.Contaminate(desease);
			spreadCount++;
		}
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

bool DeseaseSpreadSimulation::Person::WillInfect(const Desease* exposed) const
{
	// Map the acceptance factor to the inverse of the desease spread factor
	// Acceptance factor range is always 0 to 1
	// Desease spread factor range is spreadFactor to 1/10th of spreadFactor
	float probability = MapOneRangeToAnother(behavior.acceptanceFactor, 0.f, 1.f, exposed->GetSpreadFactor(), exposed->GetSpreadFactor() * 0.1f);

	std::random_device seed;
	std::mt19937 generator(seed());
	std::bernoulli_distribution distribution(probability);

	return distribution(generator);
}

void DeseaseSpreadSimulation::Person::Contaminate(const Desease* infection)
{
	desease = infection;
	seirState = Seir_State::Exposed;
	
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
	return seirState == Seir_State::Susceptible;
}

bool DeseaseSpreadSimulation::Person::isInfectious() const
{
	return seirState == Seir_State::Infectious;
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

DeseaseSpreadSimulation::Community* DeseaseSpreadSimulation::Person::GetCommunity()
{
	return community;
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::Person::GetWhereabouts()
{
	return whereabouts;
}

DeseaseSpreadSimulation::Home* DeseaseSpreadSimulation::Person::GetHome()
{
	return home;
}

DeseaseSpreadSimulation::Workplace* DeseaseSpreadSimulation::Person::GetWorkplace()
{
	return workplace;
}

DeseaseSpreadSimulation::School* DeseaseSpreadSimulation::Person::GetSchool()
{
	return school;
}

void DeseaseSpreadSimulation::Person::DeseaseCheck()
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
		}
		break;
	case DeseaseSpreadSimulation::Seir_State::Recovered:
		if (daysTillCured == 0)
		{
			desease = nullptr;
		}
		// TODO: Implement that a person can be susceptible again.
		break;
	default:
		break;
	}
}

void DeseaseSpreadSimulation::Person::SetWhereabouts(Place* newWhereabouts)
{
	whereabouts = newWhereabouts;
}

void DeseaseSpreadSimulation::Person::SetWorkplace(Workplace* newWorkplace)
{
	workplace = newWorkplace;
}

void DeseaseSpreadSimulation::Person::SetSchool(School* newSchool)
{
	school = newSchool;
}

void DeseaseSpreadSimulation::Person::SetCommunity(Community* newCommunity)
{
	community = newCommunity;
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
	if (willDie)
	{
		// decrement daysToLive and if it reached 0 the person will die
		if (--daysToLive <= 0)
		{
			alive = false;
		}
	}
}

// Comment out newDay to silence compiler warning C4100
void DeseaseSpreadSimulation::Person::OnNewDay(Day /*newDay*/)
{
	AdvanceDay();
}
