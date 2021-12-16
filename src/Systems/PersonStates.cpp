#include "pch.h"
#include "Systems/PersonStates.h"
#include "Simulation/TimeManager.h"

void DeseaseSpreadSimulation::PersonStates::Update()
{
	UpdateInEveryState();
}

DeseaseSpreadSimulation::PersonStates::PersonStates(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay)
	:
	m_lastFoodBuy(lastFoodBuy),
	m_lastHardwareBuy(lastHardwareBuy),
	m_currentDay(currentDay)
{
}

void DeseaseSpreadSimulation::PersonStates::UpdateInEveryState()
{
	// Check if it's a new day and updates all day variables when it is
	if (TimeManager::GetDay() != m_currentDay)
	{
		m_currentDay = TimeManager::GetDay();

		m_lastFoodBuy++;
		m_lastHardwareBuy++;
	}
}

DeseaseSpreadSimulation::HomeState::HomeState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay)
	:
	PersonStates(lastFoodBuy, lastHardwareBuy, currentDay)
{
}

std::unique_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::HomeState::HandleStateChange(Person& person)
{
	if (!person.isAlive())
	{
		return std::make_unique<MorgueState>();
	}

	auto time = TimeManager::GetClock();

	// If it's after 8 o'clock check if we need to get supplies or hardware, then check if we need to go to work or school
	if (time >= 8)
	{
		if (m_lastFoodBuy >= person.GetBehavior().foodBuyInterval)
		{
			return std::make_unique<FoodBuyState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}

		if (m_lastHardwareBuy >= person.GetBehavior().hardwareBuyInterval)
		{
			return std::make_unique<HardwareBuyState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}

		if (person.GetWorkplace() && time < WorkState::workFinishTime && TimeManager::isWorkday())
		{
			return std::make_unique<WorkState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}

		if (person.GetSchool() && time < SchoolState::schoolFinishTime && TimeManager::isWorkday())
		{
			return std::make_unique<SchoolState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}
	}

	return nullptr;
}

void DeseaseSpreadSimulation::HomeState::Enter(Person& person)
{
	person.SetWhereabouts(person.GetHome());
}

DeseaseSpreadSimulation::FoodBuyState::FoodBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay)
	:
	PersonStates(lastFoodBuy, lastHardwareBuy, currentDay)
{
	// Set the finish time to be 1h in the future
	buyFinishTime = TimeManager::GetClock() + 1;
}

std::unique_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::FoodBuyState::HandleStateChange(Person& person)
{
	if (!person.isAlive())
	{
		return std::make_unique<MorgueState>();
	}

	// When the buy time is over go to the hardware store if we need to, else go home
	if (TimeManager::GetClock() >= buyFinishTime)
	{
		if (m_lastHardwareBuy >= person.GetBehavior().hardwareBuyInterval)
		{
			return std::make_unique<HardwareBuyState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}
		else
		{
			return std::make_unique<HomeState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}
	}

	return nullptr;
}

void DeseaseSpreadSimulation::FoodBuyState::Enter(Person& person)
{
	person.SetWhereabouts(person.GetCommunity().GetSupplyStore());
	// Reset the last food buy
	m_lastFoodBuy = 0;
}

DeseaseSpreadSimulation::HardwareBuyState::HardwareBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay)
	:
	PersonStates(lastFoodBuy, 0, currentDay) // reset last hardware buy
{
	// Set the finish time to be 1h in the future
	buyFinishTime = TimeManager::GetClock() + 1;
}

std::unique_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::HardwareBuyState::HandleStateChange(Person& person)
{
	if (!person.isAlive())
	{
		return std::make_unique<MorgueState>();
	}

	// When the buy time is over go to the supply store if we need to, else go home
	if (TimeManager::GetClock() >= buyFinishTime)
	{
		if (m_lastFoodBuy >= person.GetBehavior().foodBuyInterval)
		{
			return std::make_unique<FoodBuyState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}
		else
		{
			return std::make_unique<HomeState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
		}
	}

	return nullptr;
}

void DeseaseSpreadSimulation::HardwareBuyState::Enter(Person& person)
{
	person.SetWhereabouts(person.GetCommunity().GetHardwareStore());
	// Reset the last hardware buy
	m_lastHardwareBuy = 0;
}

DeseaseSpreadSimulation::WorkState::WorkState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay)
	:
	PersonStates(lastFoodBuy, lastHardwareBuy, currentDay)
{
}

std::unique_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::WorkState::HandleStateChange(Person& person)
{
	if (!person.isAlive())
	{
		return std::make_unique<MorgueState>();
	}

	if (TimeManager::GetClock() >= workFinishTime)
	{
		return std::make_unique<HomeState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
	}

	return nullptr;
}

void DeseaseSpreadSimulation::WorkState::Enter(Person& person)
{
	person.SetWhereabouts(person.GetWorkplace());
}

DeseaseSpreadSimulation::SchoolState::SchoolState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay)
	:
	PersonStates(lastFoodBuy, lastHardwareBuy, currentDay)
{
}

std::unique_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::SchoolState::HandleStateChange(Person& person)
{
	if (!person.isAlive())
	{
		return std::make_unique<MorgueState>();
	}

	if (TimeManager::GetClock() >= schoolFinishTime)
	{
		return std::make_unique<HomeState>(m_lastFoodBuy, m_lastHardwareBuy, m_currentDay);
	}

	return nullptr;
}

void DeseaseSpreadSimulation::SchoolState::Enter(Person& person)
{
	person.SetWhereabouts(person.GetSchool());
}

std::unique_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::MorgueState::HandleStateChange(Person& person)
{
	return nullptr;
}

void DeseaseSpreadSimulation::MorgueState::Enter(Person& person)
{
	person.SetWhereabouts(person.GetCommunity().GetMorgue());
}
