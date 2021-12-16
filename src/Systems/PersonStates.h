#pragma once

namespace DeseaseSpreadSimulation
{
	class PersonStates
	{
	public:
		virtual std::unique_ptr<PersonStates> HandleStateChange(Person& person) = 0;
		virtual void Enter(Person& person) = 0;
		virtual void Update();

		virtual ~PersonStates() = default;
	
	protected:
		PersonStates(uint16_t lastFoodBuy = 0, uint16_t lastHardwareBuy = 0, uint16_t currentDay = 0);

		// Has to be called in every override update function
		void UpdateInEveryState();

	protected:
		// In days
		uint16_t m_lastFoodBuy = 0;
		uint16_t m_lastHardwareBuy = 0;
		
		// Current day from 1 = Monday to 7 = Sunday
		uint16_t m_currentDay = 1;
	};

	class HomeState : public PersonStates
	{
	public:
		HomeState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay);

		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	};
	
	class FoodBuyState : public PersonStates
	{
	public:
		FoodBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay);

		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	private:
		// Time in hours
		uint16_t buyFinishTime = 0;
	};

	class HardwareBuyState : public PersonStates
	{
	public:
		HardwareBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay);

		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	private:
		// Time in hours
		uint16_t buyFinishTime = 0;
	};
	
	class WorkState : public PersonStates
	{
	public:
		WorkState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay);

		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	public:
		// Time in x/24h
		static constexpr uint16_t workFinishTime = 17;
	};
	
	class SchoolState : public PersonStates
	{
	public:
		SchoolState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, uint16_t currentDay);

		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	public:
		// Time in x/24h
		static constexpr uint16_t schoolFinishTime = 15;
	};
	
	class MorgueState : public PersonStates
	{
	public:
		std::unique_ptr<PersonStates> HandleStateChange(Person& person) override;
		void Enter(Person& person) override;
	};
}
