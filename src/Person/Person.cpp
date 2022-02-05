#include "pch.h"
#include "Person/Person.h"
#include "IDGenerator/IDGenerator.h"

DeseaseSpreadSimulation::Person::Person(Age_Group age, Sex sex, PersonBehavior behavior, Community* community, Home* home)
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

void DeseaseSpreadSimulation::Person::Update(TimeManager& time, bool isNewDay)
{
	CheckNextMove(time);

	if (desease && isNewDay)
	{
		AdvanceDay();
		DeseaseCheck();
	}
}

void DeseaseSpreadSimulation::Person::Contact(Person& other)
{
	if (other.isInfectious() && isSusceptible())
	{
		if (WillInfect(other.desease))
		{
			Contaminate(other.desease);
			other.spreadCount++;
		}
	}
	else if (isInfectious() && other.isSusceptible())
	{
		if (other.WillInfect(desease))
		{
			other.Contaminate(desease);
			spreadCount++;
		}
	}
}

void DeseaseSpreadSimulation::Person::Contaminate(const Desease* infection)
{
	desease = infection;
	seirState = Seir_State::Exposed;
	
	latentPeriod = desease->IncubationPeriod();
	daysInfectious = desease->DaysInfectious();
	daysTillCured = desease->GetDeseaseDuration();

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

std::string DeseaseSpreadSimulation::Person::GetDeseaseName() const
{
	if (desease != nullptr)
	{
		return desease->GetDeseaseName();
	}
	return "";
}

bool DeseaseSpreadSimulation::Person::hasDesease() const
{
	return desease == nullptr;	
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

void DeseaseSpreadSimulation::Person::CheckNextMove(TimeManager& time)
{
	if (!alive)
	{
		// Maybe delete person and just increase a counter
		if (whereabouts->GetType() == Place_Type::Morgue)
		{
			return;
		}
		whereabouts = community->TransferToMorgue(this);
	}

	bool needFood = m_lastFoodBuy >= behavior.foodBuyInterval;
	bool needHardware = m_lastHardwareBuy >= behavior.hardwareBuyInterval;

	auto currentPlace = whereabouts->GetType();
	auto currentTime = time.GetTime();
	bool isWorkday = time.IsWorkday();

	switch (currentPlace)
	{
	case DeseaseSpreadSimulation::Place_Type::Home:
		if (needFood && currentTime >= buyTime)
		{
			GoSupplyShopping(currentTime);
		}
		else if (needHardware && currentTime >= buyTime)
		{
			GoHardwareShopping(currentTime);
		}
		else if (workplace
			&& currentTime >= workStartTime
			&& currentTime <= workFinishTime
			&& isWorkday)
		{
			whereabouts = community->TransferToWork(this);
		}
		else if (school != nullptr
			&& currentTime >= schoolStartTime
			&& currentTime <= schoolFinishTime
			&& isWorkday)
		{
			whereabouts = community->TransferToSchool(this);
		}
		break;
	case DeseaseSpreadSimulation::Place_Type::Supply:
		// When the buy time is over go to the hardware store if we need to, else go home
		if (currentTime >= buyFinishTime)
		{
			if (m_lastHardwareBuy >= behavior.hardwareBuyInterval)
			{
				GoHardwareShopping(currentTime);
			}
			else
			{
				whereabouts = community->TransferToHome(this);
			}
		}
		break;
	case DeseaseSpreadSimulation::Place_Type::Workplace:
		if (currentTime >= workFinishTime)
		{
			whereabouts = community->TransferToHome(this);
		}
		break;
	case DeseaseSpreadSimulation::Place_Type::School:
		if (currentTime >= schoolFinishTime)
		{
			whereabouts = community->TransferToHome(this);
		}
		break;
	case DeseaseSpreadSimulation::Place_Type::HardwareStore:
		if (currentTime >= buyFinishTime)
		{
			if (m_lastFoodBuy >= behavior.foodBuyInterval)
			{
				GoSupplyShopping(currentTime);
			}
			else
			{
				whereabouts = community->TransferToHome(this);
			}
		}
		break;
	case DeseaseSpreadSimulation::Place_Type::Morgue:
		return;
		break;
	default:
		break;
	}
}

void DeseaseSpreadSimulation::Person::GoSupplyShopping(uint16_t currentTime)
{
	whereabouts = community->TransferToSupplyStore(this);

	// Reset the last food buy
	m_lastFoodBuy = 0;
	buyFinishTime = currentTime + 1;
}

void DeseaseSpreadSimulation::Person::GoHardwareShopping(uint16_t currentTime)
{
	whereabouts = community->TransferToHardwareStore(this);

	// Reset the last hardware buy
	m_lastHardwareBuy = 0;
	buyFinishTime = currentTime + 1;
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
