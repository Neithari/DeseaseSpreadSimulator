#pragma once
#include "Simulation/TimeObserver.h"

namespace DeseaseSpreadSimulation
{
	class PersonStates : public TimeObserver
	{
	public:
		virtual std::unique_ptr<PersonStates> HandleStateChange(Person& person, uint16_t time) = 0;
		virtual void Enter(Person& person) = 0;
		void OnNewDay(Day newDay) override;

		virtual ~PersonStates() = default;
	
	protected:
		PersonStates(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay);

		// Has to be called in every override OnNewDay function
		void UpdateInEveryState(Day newDay);

		void SetNewWhereabouts(Person& person, Place* newWhereabouts);

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
		~HomeState();
		HomeState(const HomeState& other) = default;
		HomeState(HomeState&& other) noexcept = default;
		HomeState& operator=(const HomeState& other) = default;
		HomeState& operator=(HomeState&& other) noexcept = default;

		std::unique_ptr<PersonStates> HandleStateChange(Person& person, uint16_t time) override;
		void Enter(Person& person) override;
	private:
		// Time in hours
		uint16_t buyTime = 0u;
	};
	
	class FoodBuyState : public PersonStates
	{
	public:
		FoodBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay);
		~FoodBuyState();
		FoodBuyState(const FoodBuyState& other) = default;
		FoodBuyState(FoodBuyState&& other) noexcept = default;
		FoodBuyState& operator=(const FoodBuyState& other) = default;
		FoodBuyState& operator=(FoodBuyState&& other) noexcept = default;

		std::unique_ptr<PersonStates> HandleStateChange(Person& person, uint16_t time) override;
		void Enter(Person& person) override;
	private:
		// Time in hours
		uint16_t buyFinishTime = 0u;
	};

	class HardwareBuyState : public PersonStates
	{
	public:
		HardwareBuyState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay);
		~HardwareBuyState();
		HardwareBuyState(const HardwareBuyState& other) = default;
		HardwareBuyState(HardwareBuyState&& other) noexcept = default;
		HardwareBuyState& operator=(const HardwareBuyState& other) = default;
		HardwareBuyState& operator=(HardwareBuyState&& other) noexcept = default;

		std::unique_ptr<PersonStates> HandleStateChange(Person& person, uint16_t time) override;
		void Enter(Person& person) override;
	private:
		// Time in hours
		uint16_t buyFinishTime = 0u;
	};
	
	class WorkState : public PersonStates
	{
	public:
		WorkState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay);
		~WorkState();
		WorkState(const WorkState& other) = default;
		WorkState(WorkState&& other) noexcept = default;
		WorkState& operator=(const WorkState& other) = default;
		WorkState& operator=(WorkState&& other) noexcept = default;

		std::unique_ptr<PersonStates> HandleStateChange(Person& person, uint16_t time) override;
		void Enter(Person& person) override;
	public:
		// Time in x/24h
		static constexpr uint16_t workStartTime = 8u;
		static constexpr uint16_t workFinishTime = 17u;
	};
	
	class SchoolState : public PersonStates
	{
	public:
		SchoolState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay);
		~SchoolState();
		SchoolState(const SchoolState& other) = default;
		SchoolState(SchoolState&& other) noexcept = default;
		SchoolState& operator=(const SchoolState& other) = default;
		SchoolState& operator=(SchoolState&& other) noexcept = default;

		std::unique_ptr<PersonStates> HandleStateChange(Person& person, uint16_t time) override;
		void Enter(Person& person) override;
	public:
		// Time in x/24h
		static constexpr uint16_t schoolStartTime = 8u;
		static constexpr uint16_t schoolFinishTime = 15u;
	};
	
	class MorgueState : public PersonStates
	{
	public:
		MorgueState(uint16_t lastFoodBuy, uint16_t lastHardwareBuy, Day currentDay);
		~MorgueState();
		MorgueState(const MorgueState& other) = default;
		MorgueState(MorgueState&& other) noexcept = default;
		MorgueState& operator=(const MorgueState& other) = default;
		MorgueState& operator=(MorgueState&& other) noexcept = default;

		std::unique_ptr<PersonStates> HandleStateChange(Person& person, uint16_t time) override;
		void Enter(Person& person) override;
	};
}
