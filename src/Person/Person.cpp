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
	CheckNextMove(currentTime, isWorkday, isNewDay);
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
	return isQuarantined;
}

bool DeseaseSpreadSimulation::Person::IsTraveling() const
{
	return isTraveling;
}

bool DeseaseSpreadSimulation::Person::IsAlive() const
{
	return alive;
}

std::string DeseaseSpreadSimulation::Person::GetDeseaseName() const
{
	return infection.GetDeseaseName();
}

bool DeseaseSpreadSimulation::Person::HasRecovered() const
{
	return infection.HasRecovered();
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

uint32_t DeseaseSpreadSimulation::Person::GetSpreadCount() const
{
	return infection.GetSpreadCount();
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

void DeseaseSpreadSimulation::Person::CheckNextMove(uint16_t currentTime, bool& isWorkday, bool isNewDay)
{
	// Send the person to the morgue if not alive
	if (!alive)
	{
		// Maybe delete person and just increase a counter
		if (whereabouts->GetType() == Place_Type::Morgue)
		{
			return;
		}
		whereabouts = community->TransferToMorgue(this);
	}

	if (IsQuarantined())
	{

	}

	bool needFood = lastFoodBuy >= behavior.foodBuyInterval;
	bool needHardware = lastHardwareBuy >= behavior.hardwareBuyInterval;

	auto currentPlace = whereabouts->GetType();

	switch (currentPlace)
	{
	case DeseaseSpreadSimulation::Place_Type::Home:
		// Branch to shopping, work, school or travel here
		// Go shopping at a random time between the open hours
		if (needFood && currentTime >= shopOpenTime)
		{
			PrepareShopping();

			if (currentTime >= buyTime)
			{
				GoSupplyShopping(currentTime);
			}
		}
		else if (needHardware && currentTime >= shopOpenTime)
		{
			PrepareShopping();

			if (currentTime >= buyTime)
			{
				GoHardwareShopping(currentTime);
			}
		}
		else if (!isShoppingDay)
		{
			if (WillTravel() && currentTime >= shopOpenTime)
			{
				StartTraveling();
			}
			else if (workplace != nullptr
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
		}
		break;
	case DeseaseSpreadSimulation::Place_Type::Supply:
		// When the buy time is over go to the hardware store if we need to, else go home
		if (currentTime >= buyFinishTime)
		{
			if (lastHardwareBuy >= behavior.hardwareBuyInterval)
			{
				// No preparation needed. Just go shopping.
				GoHardwareShopping(currentTime);
			}
			else
			{
				whereabouts = community->TransferToHome(this);
			}
		}
		break;
	case DeseaseSpreadSimulation::Place_Type::Workplace:
		// Go traveling or home after the work has finished
		if (WillTravel())
		{
			StartTraveling();
		}
		else if (currentTime >= workFinishTime)
		{
			whereabouts = community->TransferToHome(this);
		}
		break;
	case DeseaseSpreadSimulation::Place_Type::School:
		// Go home after the school has finished
		if (currentTime >= schoolFinishTime)
		{
			whereabouts = community->TransferToHome(this);
		}
		break;
	case DeseaseSpreadSimulation::Place_Type::HardwareStore:
		// When the buy time is over go to the supply store if we need to, else go home
		if (currentTime >= buyFinishTime)
		{
			if (lastFoodBuy >= behavior.foodBuyInterval)
			{
				// No preparation needed. Just go shopping.
				GoSupplyShopping(currentTime);
			}
			else
			{
				whereabouts = community->TransferToHome(this);
			}
		}
		break;
	case DeseaseSpreadSimulation::Place_Type::Morgue:
		// We stay here for ever
		return;
		break;
	case DeseaseSpreadSimulation::Place_Type::Travel:
		// Return from travel to home is an increasing chance per day traveling. 
		if (isNewDay)
		{
			travelDays++;
			if (Random::Percent<float>() <= (baseTravelReturnChance * (travelDays / static_cast<float>(3))))
			{
				isTraveling = false;
				travelDays = 0u;
				whereabouts = community->TransferToHome(this);
			}
		}
		return;
		break;
	default:
		break;
	}
}

void DeseaseSpreadSimulation::Person::PrepareShopping()
{
	if (!isShoppingDay)
	{
		buyTime = Random::UniformIntRange(shopOpenTime, shopCloseTime);

		isShoppingDay = true;
	}
}

void DeseaseSpreadSimulation::Person::GoSupplyShopping(uint16_t currentTime)
{
	whereabouts = community->TransferToSupplyStore(this);

	// Reset the last food buy
	lastFoodBuy = 0;
	buyFinishTime = currentTime + 1;
}

void DeseaseSpreadSimulation::Person::GoHardwareShopping(uint16_t currentTime)
{
	whereabouts = community->TransferToHardwareStore(this);

	// Reset the last hardware buy
	lastHardwareBuy = 0;
	buyFinishTime = currentTime + 1;
}

bool DeseaseSpreadSimulation::Person::WillTravel() const
{
	return Random::Percent<float>() <= behavior.travelNeed;
}

void DeseaseSpreadSimulation::Person::StartTraveling()
{
	whereabouts = community->TransferToTravelLocation(this);
	isTraveling = true;
}

void DeseaseSpreadSimulation::Person::StartQuarantine()
{
	isQuarantined = true;
}

void DeseaseSpreadSimulation::Person::EndQuarantine()
{
	isQuarantined = false;
}
