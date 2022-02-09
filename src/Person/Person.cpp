#include "pch.h"
#include "Person/Person.h"
#include "IDGenerator/IDGenerator.h"

DeseaseSpreadSimulation::Person::Person(Age_Group age, Sex sex, PersonBehavior behavior, Community* community, Home* home)
	:
	id(IDGenerator::IDGenerator<Person>::GetNextID()),
	age(age),
	sex(sex),
	behavior(std::move(behavior)),
	community(community),
	home(home),
	whereabouts(home)
{
	if (whereabouts != nullptr)
	{
		whereabouts->AddPerson(this);
	}
}

void DeseaseSpreadSimulation::Person::Update(uint16_t currentTime, bool isWorkday, bool isNewDay)
{
	CheckNextMove(currentTime, isWorkday);
	infection.Update(*this, isNewDay);
}

void DeseaseSpreadSimulation::Person::Contact(Person& other)
{
	if (other.IsInfectious() && IsSusceptible())
	{
		if (infection.WillInfect(other.infection.GetDesease(), behavior.acceptanceFactor))
		{
			infection.Contaminate(other.infection.GetDesease(), age);
			other.infection.IncreaseSpreadCount();
		}
	}
	else if (IsInfectious() && other.IsSusceptible())
	{
		if (other.infection.WillInfect(infection.GetDesease(), other.behavior.acceptanceFactor))
		{
			other.infection.Contaminate(infection.GetDesease(), other.age);
			infection.IncreaseSpreadCount();
		}
	}
}

void DeseaseSpreadSimulation::Person::Contaminate(const Desease* desease)
{
	infection.Contaminate(desease, age);
}

void DeseaseSpreadSimulation::Person::Kill()
{
	alive = false;
}

bool DeseaseSpreadSimulation::Person::Travel()
{
	return Random::Percent<float>() <= behavior.travelNeed;
}

bool DeseaseSpreadSimulation::Person::IsSusceptible() const
{
	return infection.IsSusceptible();
}

bool DeseaseSpreadSimulation::Person::IsInfectious() const
{
	return infection.IsInfectious();
}

bool DeseaseSpreadSimulation::Person::IsQuarantined() const
{
	return infection.IsQuarantined();
}

bool DeseaseSpreadSimulation::Person::IsAlive() const
{
	return alive;
}

std::string DeseaseSpreadSimulation::Person::GetDeseaseName() const
{
	return infection.GetDeseaseName();
}

bool DeseaseSpreadSimulation::Person::HasDesease() const
{
	return infection.HasDesease();
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

void DeseaseSpreadSimulation::Person::CheckNextMove(uint16_t& currentTime, bool& isWorkday)
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
