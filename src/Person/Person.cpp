#include "pch.h"
#include "Person/Person.h"
#include "IDGenerator/IDGenerator.h"

DiseaseSpreadSimulation::Person::Person(Age_Group age, Sex sex, PersonBehavior behavior, Community* community, Home* home)
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

void DiseaseSpreadSimulation::Person::Update(uint16_t currentTime, bool isWorkday, bool isNewDay)
{
	CheckNextMove(currentTime, isWorkday, isNewDay);
	infection.Update(*this, isNewDay);
}

void DiseaseSpreadSimulation::Person::Contact(Person& other)
{
	if (other.IsInfectious() && IsSusceptible())
	{
		if (infection.WillInfect(other.infection.GetDisease(), behavior.acceptanceFactor))
		{
			infection.Contaminate(other.infection.GetDisease(), age);
			other.infection.IncreaseSpreadCount();
		}
	}
	else if (IsInfectious() && other.IsSusceptible())
	{
		if (other.infection.WillInfect(infection.GetDisease(), other.behavior.acceptanceFactor))
		{
			other.infection.Contaminate(infection.GetDisease(), other.age);
			infection.IncreaseSpreadCount();
		}
	}
}

void DiseaseSpreadSimulation::Person::Contaminate(const Disease* disease)
{
	infection.Contaminate(disease, age);
}

void DiseaseSpreadSimulation::Person::Kill()
{
	alive = false;
}

bool DiseaseSpreadSimulation::Person::IsSusceptible() const
{
	return infection.IsSusceptible();
}

bool DiseaseSpreadSimulation::Person::IsInfectious() const
{
	return infection.IsInfectious();
}

bool DiseaseSpreadSimulation::Person::IsQuarantined() const
{
	return isQuarantined;
}

bool DiseaseSpreadSimulation::Person::IsTraveling() const
{
	return isTraveling;
}

bool DiseaseSpreadSimulation::Person::IsAlive() const
{
	return alive;
}

std::string DiseaseSpreadSimulation::Person::GetDiseaseName() const
{
	return infection.GetDiseaseName();
}

bool DiseaseSpreadSimulation::Person::HasRecovered() const
{
	return infection.HasRecovered();
}

bool DiseaseSpreadSimulation::Person::HasDisease() const
{
	return infection.HasDisease();
}

uint32_t DiseaseSpreadSimulation::Person::GetID() const
{
	return id;
}

DiseaseSpreadSimulation::Age_Group DiseaseSpreadSimulation::Person::GetAgeGroup() const
{
	return age;
}

DiseaseSpreadSimulation::Sex DiseaseSpreadSimulation::Person::GetSex() const
{
	return sex;
}

const DiseaseSpreadSimulation::PersonBehavior& DiseaseSpreadSimulation::Person::GetBehavior() const
{
	return behavior;
}

uint32_t DiseaseSpreadSimulation::Person::GetSpreadCount() const
{
	return infection.GetSpreadCount();
}

const DiseaseSpreadSimulation::Disease* DiseaseSpreadSimulation::Person::GetDisease() const
{
	return infection.GetDisease();
}

DiseaseSpreadSimulation::Community* DiseaseSpreadSimulation::Person::GetCommunity()
{
	return community;
}

DiseaseSpreadSimulation::Place* DiseaseSpreadSimulation::Person::GetWhereabouts()
{
	return whereabouts;
}

DiseaseSpreadSimulation::Home* DiseaseSpreadSimulation::Person::GetHome()
{
	return home;
}

DiseaseSpreadSimulation::Workplace* DiseaseSpreadSimulation::Person::GetWorkplace()
{
	return workplace;
}

DiseaseSpreadSimulation::School* DiseaseSpreadSimulation::Person::GetSchool()
{
	return school;
}

void DiseaseSpreadSimulation::Person::SetWorkplace(Workplace* newWorkplace)
{
	workplace = newWorkplace;
}

void DiseaseSpreadSimulation::Person::SetSchool(School* newSchool)
{
	school = newSchool;
}

void DiseaseSpreadSimulation::Person::SetCommunity(Community* newCommunity)
{
	community = newCommunity;
}

void DiseaseSpreadSimulation::Person::SetHome(Home* newHome)
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

void DiseaseSpreadSimulation::Person::ChangeBehavior(PersonBehavior newBehavior)
{
	behavior = newBehavior;
}

void DiseaseSpreadSimulation::Person::CheckNextMove(uint16_t currentTime, bool& isWorkday, bool isNewDay)
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

	// When we are quarantined do nothing untill we have recovered
	if (IsQuarantined())
	{
		if (infection.HasRecovered() && currentTime >= shopOpenTime)
		{
			community->TestStation(this);
		}
		return;
	}

	// Test if we have symptoms
	if (infection.HasSymptoms() && currentTime >= shopOpenTime)
	{
		// When our acceptance factor is too low, we decide random if we test or not
		if (behavior.acceptanceFactor <= 0.6f && Random::Percent<float>() > behavior.acceptanceFactor)
		{
			return;
		}

		community->TestStation(this);
		return;
	}

	bool needFood = lastFoodBuy >= behavior.foodBuyInterval;
	bool needHardware = lastHardwareBuy >= behavior.hardwareBuyInterval;

	auto currentPlace = whereabouts->GetType();

	switch (currentPlace)
	{
	case DiseaseSpreadSimulation::Place_Type::Home:
		// Branch to shopping, work, school, travel or test here
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
	case DiseaseSpreadSimulation::Place_Type::Supply:
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
	case DiseaseSpreadSimulation::Place_Type::Workplace:
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
	case DiseaseSpreadSimulation::Place_Type::School:
		// Go home after the school has finished
		if (currentTime >= schoolFinishTime)
		{
			whereabouts = community->TransferToHome(this);
		}
		break;
	case DiseaseSpreadSimulation::Place_Type::HardwareStore:
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
	case DiseaseSpreadSimulation::Place_Type::Morgue:
		// We stay here for ever
		return;
		break;
	case DiseaseSpreadSimulation::Place_Type::Travel:
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

void DiseaseSpreadSimulation::Person::PrepareShopping()
{
	if (!isShoppingDay)
	{
		buyTime = Random::UniformIntRange(shopOpenTime, shopCloseTime);

		isShoppingDay = true;
	}
}

void DiseaseSpreadSimulation::Person::GoSupplyShopping(uint16_t currentTime)
{
	whereabouts = community->TransferToSupplyStore(this);

	// Reset the last food buy
	lastFoodBuy = 0;
	buyFinishTime = currentTime + 1;
}

void DiseaseSpreadSimulation::Person::GoHardwareShopping(uint16_t currentTime)
{
	whereabouts = community->TransferToHardwareStore(this);

	// Reset the last hardware buy
	lastHardwareBuy = 0;
	buyFinishTime = currentTime + 1;
}

bool DiseaseSpreadSimulation::Person::WillTravel() const
{
	return Random::Percent<float>() <= behavior.travelNeed;
}

void DiseaseSpreadSimulation::Person::StartTraveling()
{
	whereabouts = community->TransferToTravelLocation(this);
	isTraveling = true;
}

void DiseaseSpreadSimulation::Person::StartQuarantine()
{
	isQuarantined = true;
}

void DiseaseSpreadSimulation::Person::EndQuarantine()
{
	isQuarantined = false;
}
