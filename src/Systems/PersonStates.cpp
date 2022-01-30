#include "pch.h"
#include "Systems/PersonStates.h"
#include "Simulation/TimeManager.h"

void DeseaseSpreadSimulation::PersonStates::OnNewDay(Day newDay)
{
	UpdateInEveryState(newDay);
}

DeseaseSpreadSimulation::PersonStates::PersonStates(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay)
	:
	m_lastFoodBuy(lastFoodBuy),
	m_lastHardwareBuy(lastHardwareBuy),
	m_currentDay(currentDay)
{
}

void DeseaseSpreadSimulation::PersonStates::UpdateInEveryState(Day newDay)
{
	m_currentDay = newDay;

	m_lastFoodBuy++;
	m_lastHardwareBuy++;
}

void DeseaseSpreadSimulation::PersonStates::SetNewWhereabouts(Person& person, Place* newWhereabouts)
{
	auto whereabouts = person.GetWhereabouts();
	if (whereabouts && newWhereabouts)
	{
		// Remove the person from current whereabout
		whereabouts->RemovePerson(&person);

		// Send the person to the new place
		person.SetWhereabouts(newWhereabouts);
		person.GetWhereabouts()->AddPerson(&person);
	}
}

DeseaseSpreadSimulation::HomeState::HomeState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay)
	:
	PersonStates(lastFoodBuy, lastHardwareBuy, currentDay)
{
	TimeManager::Instance().AddObserver(this);
}

DeseaseSpreadSimulation::HomeState::~HomeState()
{
	TimeManager::Instance().RemoveObserver(this);
}

std::shared_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::HomeState::HandleStateChange(Person& person, uint16_t time)
{
	if (!person.isAlive())
	{
		return std::make_shared<MorgueState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
	}

	bool needFood = m_lastFoodBuy >= person.GetBehavior().foodBuyInterval;
	bool needHardware = m_lastHardwareBuy >= person.GetBehavior().hardwareBuyInterval;

	if (needFood || needHardware)
	{
		if (needFood && time >= buyTime)
		{
			return std::make_shared<FoodBuyState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}

		if (needHardware && time >= buyTime)
		{
			return std::make_shared<HardwareBuyState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}
	}
	else
	{
		if (person.GetWorkplace() && time >= WorkState::workStartTime && time <= WorkState::workFinishTime && TimeManager::IsWorkday(m_currentDay))
		{
			return std::make_shared<WorkState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}

		if (person.GetSchool() && time >= SchoolState::schoolStartTime && time <= SchoolState::schoolFinishTime && TimeManager::IsWorkday(m_currentDay))
		{
			return std::make_shared<SchoolState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}
	}

	return nullptr;
}

void DeseaseSpreadSimulation::HomeState::Enter(Person& person)
{
	SetNewWhereabouts(person, person.GetHome());

	// Set random buy time
	std::random_device seed;
	std::mt19937 generator(seed());
	std::uniform_int_distribution<uint16_t> distribution(8, 19);
	buyTime = distribution(generator);
}

DeseaseSpreadSimulation::FoodBuyState::FoodBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay)
	:
	PersonStates(lastFoodBuy, lastHardwareBuy, currentDay)
{
	TimeManager::Instance().AddObserver(this);
	// Set the finish time to be 1h in the future
	buyFinishTime = TimeManager::Instance().GetTime() + 1;
}

DeseaseSpreadSimulation::FoodBuyState::~FoodBuyState()
{
	TimeManager::Instance().RemoveObserver(this);
}

std::shared_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::FoodBuyState::HandleStateChange(Person& person, uint16_t time)
{
	if (!person.isAlive())
	{
		return std::make_shared<MorgueState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
	}

	// When the buy time is over go to the hardware store if we need to, else go home
	if (time >= buyFinishTime)
	{
		if (m_lastHardwareBuy >= person.GetBehavior().hardwareBuyInterval)
		{
			return std::make_shared<HardwareBuyState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}
		else
		{
			return std::make_shared<HomeState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}
	}

	return nullptr;
}

void DeseaseSpreadSimulation::FoodBuyState::Enter(Person& person)
{
	SetNewWhereabouts(person, person.GetCommunity()->GetSupplyStore());

	// Reset the last food buy
	m_lastFoodBuy = 0;
}

DeseaseSpreadSimulation::HardwareBuyState::HardwareBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay)
	:
	PersonStates(lastFoodBuy, lastHardwareBuy, currentDay)
{
	// Set the finish time to be 1h in the future
	buyFinishTime = TimeManager::Instance().GetTime() + 1;
}

DeseaseSpreadSimulation::HardwareBuyState::~HardwareBuyState()
{
	TimeManager::Instance().RemoveObserver(this);
}

std::shared_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::HardwareBuyState::HandleStateChange(Person& person, uint16_t time)
{
	if (!person.isAlive())
	{
		return std::make_shared<MorgueState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
	}

	// When the buy time is over go to the supply store if we need to, else go home
	if (time >= buyFinishTime)
	{
		if (m_lastFoodBuy >= person.GetBehavior().foodBuyInterval)
		{
			return std::make_shared<FoodBuyState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}
		else
		{
			return std::make_shared<HomeState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}
	}

	return nullptr;
}

void DeseaseSpreadSimulation::HardwareBuyState::Enter(Person& person)
{
	SetNewWhereabouts(person, person.GetCommunity()->GetHardwareStore());

	// Reset the last hardware buy
	m_lastHardwareBuy = 0;
}

DeseaseSpreadSimulation::WorkState::WorkState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay)
	:
	PersonStates(lastFoodBuy, lastHardwareBuy, currentDay)
{
	TimeManager::Instance().AddObserver(this);
}

DeseaseSpreadSimulation::WorkState::~WorkState()
{
	TimeManager::Instance().RemoveObserver(this);
}

std::shared_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::WorkState::HandleStateChange(Person& person, uint16_t time)
{
	if (!person.isAlive())
	{
		return std::make_shared<MorgueState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
	}

	if (time >= workFinishTime)
	{
		return std::make_shared<HomeState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
	}

	return nullptr;
}

void DeseaseSpreadSimulation::WorkState::Enter(Person& person)
{
	SetNewWhereabouts(person, person.GetWorkplace());
}

DeseaseSpreadSimulation::SchoolState::SchoolState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay)
	:
	PersonStates(lastFoodBuy, lastHardwareBuy, currentDay)
{
	TimeManager::Instance().AddObserver(this);
}

DeseaseSpreadSimulation::SchoolState::~SchoolState()
{
	TimeManager::Instance().RemoveObserver(this);
}

std::shared_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::SchoolState::HandleStateChange(Person& person, uint16_t time)
{
	if (!person.isAlive())
	{
		return std::make_shared<MorgueState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
	}

	if (time >= schoolFinishTime)
	{
		return std::make_shared<HomeState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
	}

	return nullptr;
}

void DeseaseSpreadSimulation::SchoolState::Enter(Person& person)
{
	SetNewWhereabouts(person, person.GetSchool());
}

DeseaseSpreadSimulation::MorgueState::MorgueState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay)
	:
	PersonStates(lastFoodBuy, lastHardwareBuy, currentDay)
{
	TimeManager::Instance().AddObserver(this);
}

DeseaseSpreadSimulation::MorgueState::~MorgueState()
{
	TimeManager::Instance().RemoveObserver(this);
}

// Commenting out person and time to silence compiler warning C4100
std::shared_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::MorgueState::HandleStateChange(Person& /*person*/, uint16_t /*time*/)
{
	// There is no return to another place from a morgue right now
	return nullptr;
}

void DeseaseSpreadSimulation::MorgueState::Enter(Person& person)
{
	SetNewWhereabouts(person, person.GetCommunity()->GetMorgue());
}
