#pragma once
#include "Places/Places.h"
#include "Simulation/TimeManager.h"
#include "Person/PersonBehavior.h"
#include <type_traits>

namespace DeseaseSpreadSimulation
{
	class Community;

	class Person
	{
	public:
		// Create a Person with age, sex, community and set it's home and whereabout to home
		Person(Age_Group age, Sex sex, PersonBehavior behavior, Community* community, Home* home = nullptr);
		
		auto operator<=>(const Person& rhs) const
		{
			if (id < rhs.id) return -1;
			if (id > rhs.id) return 1;
			return 0;
		};
		inline bool operator==(const Person& rhs) const
		{
			return id == rhs.id;
		}

		void Update(TimeManager& time, bool isNewDay);

		void Contact(Person& other);
		std::string GetDeseaseName() const;
		bool WillInfect(const Desease* exposed) const;
		void Contaminate(const Desease* infection);
		// Advance daysTillOutbreak, daysContagious, daysTillCured, daysToLive by a delta time
		void AdvanceDay();

		bool isSusceptible() const;
		bool isInfectious() const;
		bool isQuarantined() const;
		bool isAlive() const;
		bool hasDesease(const std::string& deseaseName) const;

		uint32_t GetID() const;
		Age_Group GetAgeGroup() const;
		Sex GetSex() const;
		const PersonBehavior& GetBehavior() const;
		Community* GetCommunity();

		Place* GetWhereabouts();
		Home* GetHome() ;
		Workplace* GetWorkplace();
		School* GetSchool();
		
		void SetWhereabouts(Place* newWhereabouts);
		void SetHome(Home* newHome);
		void SetWorkplace(Workplace* newWorkplace);
		void SetSchool(School* newSchool);
		void SetCommunity(Community* newCommunity);
		
		void ChangeBehavior(PersonBehavior newBehavior);
		void Move(Place* destination);

	private:
		void DeseaseCheck();

		template <typename T, typename = typename std::enable_if_t<std::is_floating_point<T>::value>>
		T MapOneRangeToAnother(T value, T fromRangeMin, T fromRangeMax, T toRangeMin, T toRangeMax) const
		{
			return toRangeMin + (((value - fromRangeMin) * (toRangeMax - toRangeMin)) / (fromRangeMax - fromRangeMin));
		}

		void CheckNextMove(TimeManager& time);
		void GoSupplyShopping(uint16_t currentTime);
		void GoHardwareShopping(uint16_t currentTime);

	private:
		uint32_t id;
		Age_Group age;
		Sex sex;
		PersonBehavior behavior;
		bool alive = true;

		// Not const because we will add ourself to the places
		Community* community;
		Home* home;
		Place* whereabouts;
		Workplace* workplace = nullptr;
		School* school = nullptr;

		// Desease Stuff
		//-----------------------------------------
		Seir_State seirState = Seir_State::Susceptible;
		bool quarantined = false;
		unsigned int spreadCount = 0;

		const Desease* desease = nullptr;
		unsigned int latentPeriod = 0;
		unsigned int daysInfectious = 0;
		unsigned int daysTillCured = 0;
		unsigned int daysToLive = 0;
		bool willDie = false;
		//-----------------------------------------

		// State things

		// In days
		uint16_t m_lastFoodBuy = 0u;
		uint16_t m_lastHardwareBuy = 0u;
		uint16_t buyTime = 0u;
		uint16_t buyFinishTime = 0u;
		// Time in x/24h
		static constexpr uint16_t workStartTime = 8u;
		static constexpr uint16_t workFinishTime = 17u;
		static constexpr uint16_t schoolStartTime = 8u;
		static constexpr uint16_t schoolFinishTime = 15u;
	};
}
