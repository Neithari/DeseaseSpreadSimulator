#include "Person/Person.h"
#include "Disease/Disease.h"
#include "IDGenerator/IDGenerator.h"
#include "Places/Community.h"
#include "Disease/DiseaseContainment.h"
#include "RandomNumbers.h"

DiseaseSpreadSimulation::Person::Person(Age_Group age, Sex sex, PersonBehavior behavior, Community* community, Home* home)
	: id(IDGenerator::IDGenerator<Person>::GetNextID()),
	  m_age(age),
	  m_sex(sex),
	  m_behavior(behavior),
	  m_community(community),
	  m_home(home),
	  whereabouts(home)
{
	if (whereabouts != nullptr)
	{
		whereabouts->AddPerson(this);
	}
}

void DiseaseSpreadSimulation::Person::Update(uint32_t currentTime, bool isWorkday, bool isNewDay)
{
	CheckNextMove(currentTime, isWorkday, isNewDay);
	infection.Update(*this, isNewDay);
}

void DiseaseSpreadSimulation::Person::Contact(Person& other)
{
	if (IsInfectious() && other.IsSusceptible())
	{
		if (other.infection.WillInfect(infection, other.m_behavior.acceptanceFactor, other.m_community))
		{
			SpreadDisease(*this, other);
		}
	}
	else if (other.IsInfectious() && IsSusceptible())
	{
		if (infection.WillInfect(other.infection, m_behavior.acceptanceFactor, m_community))
		{
			SpreadDisease(other, *this);
		}
	}
}

void DiseaseSpreadSimulation::Person::Contaminate(const Disease* disease)
{
	infection.Contaminate(disease, m_age);
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

const std::string& DiseaseSpreadSimulation::Person::GetDiseaseName() const
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
	return m_age;
}

DiseaseSpreadSimulation::Sex DiseaseSpreadSimulation::Person::GetSex() const
{
	return m_sex;
}

const DiseaseSpreadSimulation::PersonBehavior& DiseaseSpreadSimulation::Person::GetBehavior() const
{
	return m_behavior;
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
	return m_community;
}

DiseaseSpreadSimulation::Place* DiseaseSpreadSimulation::Person::GetWhereabouts()
{
	return whereabouts;
}

DiseaseSpreadSimulation::Home* DiseaseSpreadSimulation::Person::GetHome()
{
	return m_home;
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

void DiseaseSpreadSimulation::Person::SetCanWorkFromHome(bool set)
{
	if (hasCriticalInfrastructureJob)
	{
		hasCriticalInfrastructureJob = !hasCriticalInfrastructureJob;
	}

	canWorkFromHome = set;
}

void DiseaseSpreadSimulation::Person::SetHasCriticalInfrastructureJob(bool set)
{
	if (canWorkFromHome)
	{
		canWorkFromHome = !canWorkFromHome;
	}

	hasCriticalInfrastructureJob = set;
}

void DiseaseSpreadSimulation::Person::SetSchool(School* newSchool)
{
	school = newSchool;
}

void DiseaseSpreadSimulation::Person::SetCommunity(Community* newCommunity)
{
	m_community = newCommunity;
}

void DiseaseSpreadSimulation::Person::SetHome(Home* newHome)
{
	m_home = newHome;
	// Check if the person is already somewhere.
	if (whereabouts == nullptr)
	{
		// If not set it's whereabouts to home...
		whereabouts = m_home;
		// ...and put the person in it's home
		m_home->AddPerson(this);
	}
}

void DiseaseSpreadSimulation::Person::ChangeBehavior(PersonBehavior newBehavior)
{
	m_behavior = newBehavior;
}

// TODO: Refactor this complex function. Silence warnings untill then
// NOLINTNEXTLINE(readability-function-cognitive-complexity)
void DiseaseSpreadSimulation::Person::CheckNextMove(uint32_t currentTime, bool& isWorkday, bool isNewDay)
{
	auto currentPlace = whereabouts->GetType();

	// Send the person to the morgue if not alive
	if (!alive)
	{
		// TODO: Maybe delete person and just increase a counter
		if (currentPlace == Place_Type::Morgue)
		{
			return;
		}
		whereabouts = m_community->TransferToMorgue(this);
	}

	// When we are quarantined do nothing untill we have recovered
	if (IsQuarantined())
	{
		if (infection.HasRecovered() && currentTime >= shopOpenTime)
		{
			m_community->TestStation(this);
		}
		return;
	}

	// Test if we have symptoms
	if (infection.HasSymptoms() && currentTime >= shopOpenTime)
	{
		// When our acceptance factor is too low, we decide random if we test or not
		if (m_behavior.acceptanceFactor <= PersonBehavior::acceptanceFactorThreshold && Random::Percent<float>() > m_behavior.acceptanceFactor)
		{
			return;
		}

		m_community->TestStation(this);
		return;
	}

	bool needFood = lastFoodBuy >= m_behavior.foodBuyInterval;
	bool needHardware = lastHardwareBuy >= m_behavior.hardwareBuyInterval;
	const auto& containmentMeasures = m_community->ContainmentMeasures();

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
		// Hardware shopping is suspended during a lockdown
		else if (needHardware
				 && currentTime >= shopOpenTime
				 && !containmentMeasures.ShopsAreClosed()
				 && !containmentMeasures.IsLockdown())
		{
			PrepareShopping();

			if (currentTime >= buyTime)
			{
				GoHardwareShopping(currentTime);
			}
		}
		else if (!isShoppingDay)
		{
			// No traveling during a lockdown
			if (WillTravel()
				&& currentTime >= shopOpenTime
				&& !containmentMeasures.IsLockdown())
			{
				StartTraveling();
			}
			else if (workplace != nullptr
					 && currentTime >= workStartTime
					 && currentTime <= workFinishTime
					 && isWorkday)
			{
				if (containmentMeasures.WorkingFromHome() && canWorkFromHome)
				{
					// 50% of working people are allowed to go to work when there is a working from home mandate.
					// Reflecting jobs that are not capable of work from home
					return;
				}
				else if (containmentMeasures.IsLockdown() && !hasCriticalInfrastructureJob)
				{
					// During a lockdown only 10% of people are allowed to go to work
					// Reflecting jobs that are mandatory to supply people
					return;
				}
				else
				{
					whereabouts = m_community->TransferToWork(this);
				}
			}
			// Schools will close when there is a work form home mandate and when there is a lockdown
			else if (school != nullptr
					 && currentTime >= schoolStartTime
					 && currentTime <= schoolFinishTime
					 && isWorkday
					 && !containmentMeasures.WorkingFromHome()
					 && !containmentMeasures.IsLockdown())
			{
				whereabouts = m_community->TransferToSchool(this);
			}
		}
		break;
	case DiseaseSpreadSimulation::Place_Type::Supply:
		// When the buy time is over go to the hardware store if we need to, else go home
		if (currentTime >= buyFinishTime)
		{
			// Hardware stores are closed during a lockdown
			if (lastHardwareBuy >= m_behavior.hardwareBuyInterval
				&& !containmentMeasures.ShopsAreClosed()
				&& !containmentMeasures.IsLockdown())
			{
				// No preparation needed. Just go shopping.
				GoHardwareShopping(currentTime);
			}
			else
			{
				whereabouts = m_community->TransferToHome(this);
			}
		}
		break;
	case DiseaseSpreadSimulation::Place_Type::Workplace:
		// Go traveling or home after the work has finished
		// No traveling during a lockdown
		if (WillTravel() && !containmentMeasures.IsLockdown())
		{
			StartTraveling();
		}
		else if (currentTime >= workFinishTime)
		{
			whereabouts = m_community->TransferToHome(this);
		}
		break;
	case DiseaseSpreadSimulation::Place_Type::School:
		// Go home after the school has finished
		if (currentTime >= schoolFinishTime)
		{
			whereabouts = m_community->TransferToHome(this);
		}
		break;
	case DiseaseSpreadSimulation::Place_Type::HardwareStore:
		// When the buy time is over go to the supply store if we need to, else go home
		if (currentTime >= buyFinishTime)
		{
			if (lastFoodBuy >= m_behavior.foodBuyInterval)
			{
				// No preparation needed. Just go shopping.
				GoSupplyShopping(currentTime);
			}
			else
			{
				whereabouts = m_community->TransferToHome(this);
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
			if (Random::Percent<float>() <= (baseTravelReturnChance * (static_cast<float>(travelDays) / static_cast<float>(3))))
			{
				isTraveling = false;
				travelDays = 0U;
				whereabouts = m_community->TransferToHome(this);
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

void DiseaseSpreadSimulation::Person::GoSupplyShopping(uint32_t currentTime)
{
	whereabouts = m_community->TransferToSupplyStore(this);

	// Reset the last food buy
	lastFoodBuy = 0;
	buyFinishTime = currentTime + 1;
}

void DiseaseSpreadSimulation::Person::GoHardwareShopping(uint32_t currentTime)
{
	whereabouts = m_community->TransferToHardwareStore(this);

	// Reset the last hardware buy
	lastHardwareBuy = 0;
	buyFinishTime = currentTime + 1;
}

bool DiseaseSpreadSimulation::Person::WillTravel() const
{
	return Random::Percent<float>() <= m_behavior.travelNeed;
}

void DiseaseSpreadSimulation::Person::StartTraveling()
{
	whereabouts = m_community->TransferToTravelLocation(this);
	isTraveling = true;
}

void DiseaseSpreadSimulation::Person::SpreadDisease(Person& spreader, Person& other)
{
	other.infection.Contaminate(spreader.infection.GetDisease(), other.m_age);
	spreader.infection.IncreaseSpreadCount();
}

void DiseaseSpreadSimulation::Person::StartQuarantine()
{
	isQuarantined = true;
}

void DiseaseSpreadSimulation::Person::EndQuarantine()
{
	isQuarantined = false;
}
