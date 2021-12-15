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
	// check if it's a new day and updates all day variables when it is
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

DeseaseSpreadSimulation::FoodBuyState::FoodBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay)
	:
	PersonStates(0, lastHardwareBuy, currentDay) // reset last food buy
{
	// set the finish time to be 1h in the future
	buyFinishTime = TimeManager::GetClock() + 1;
}

std::unique_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::FoodBuyState::HandleStateChange(Person& person)
{
	if (!person.isAlive())
	{
		return std::make_unique<MorgueState>();
	}

	// when the buy time is over go to the hardware store if we need to, else go home
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

DeseaseSpreadSimulation::HardwareBuyState::HardwareBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay)
	:
	PersonStates(lastFoodBuy, 0, currentDay) // reset last hardware buy
{
	// set the finish time to be 1h in the future
	buyFinishTime = TimeManager::GetClock() + 1;
}

std::unique_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::HardwareBuyState::HandleStateChange(Person& person)
{
	if (!person.isAlive())
	{
		return std::make_unique<MorgueState>();
	}

	// when the buy time is over go to the supply store if we need to, else go home
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

std::unique_ptr<DeseaseSpreadSimulation::PersonStates> DeseaseSpreadSimulation::MorgueState::HandleStateChange(Person& person)
{
	return nullptr;
}
