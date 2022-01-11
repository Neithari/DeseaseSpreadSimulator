#pragma once
#include "Simulation/TimeObserver.h"

namespace DeseaseSpreadSimulation
{
	class PersonStates : public TimeObserver
	{
	public:
		virtual std::unique_ptr<PersonStates> HandleStateChange(Person& person) = 0;
		virtual void Enter(Person& person) = 0;
		void OnNewDay(Day currentDay) override;

		virtual ~PersonStates() = default;
	
	protected:
		PersonStates(uint16_t lastFoodBuy = 0u, uint16_t lastHardwareBuy = 0u, Day currentDay = Day::Monday);

		// Has to be called in every override OnNewDay function
		void UpdateInEveryState(Day currenDay);

	protected:
		// In days
		uint16_t m_lastFoodBuy = 0u;
		uint16_t m_lastHardwareBuy = 0u;
		
		Day m_currentDay = Day::Monday;
	};

	class HomeState : public PersonStates
	{
	public:
		HomeState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay);

		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	};
	
	class FoodBuyState : public PersonStates
	{
	public:
		FoodBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay);

		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	private:
		// Time in hours
		uint16_t buyFinishTime = 0u;
	};

	class HardwareBuyState : public PersonStates
	{
	public:
		HardwareBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay);

		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	private:
		// Time in hours
		uint16_t buyFinishTime = 0u;
	};
	
	class WorkState : public PersonStates
	{
	public:
		WorkState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay);

		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	public:
		// Time in x/24h
		static constexpr uint16_t workFinishTime = 17u;
	};
	
	class SchoolState : public PersonStates
	{
	public:
		SchoolState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay);

		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	public:
		// Time in x/24h
		static constexpr uint16_t schoolFinishTime = 15u;
	};
	
	class MorgueState : public PersonStates
	{
	public:
		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	};
}
