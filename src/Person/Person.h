#pragma once
#include "Places/Places.h"
#include "Person/PersonBehavior.h"
#include "Simulation/TimeObserver.h"
#include "Systems/PersonStates.h"

namespace DeseaseSpreadSimulation
{
	class Community;

	class Person : public TimeObserver
	{
	public:
		// Create a Person with age, sex, community and set it's home and whereabout to home
		Person(Age_Group age, Sex sex, PersonBehavior behavior, const Community& community, Home* home = nullptr);
		~Person();
		
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

		void Update();

		void Contact(Person& other);
		std::string GetDeseaseName() const;
		void Contaminate(const Desease* infection);
		// Advance daysTillOutbreak, daysContagious, daysTillCured, daysToLive by a delta time
		void AdvanceDay();
		void OnNewDay(Day currentDay) override;

		bool isSusceptible() const;
		bool isInfectious() const;
		bool isQuarantined() const;
		bool isAlive() const;
		bool hasDesease(const std::string& deseaseName) const;

		uint32_t GetID() const;
		Age_Group GetAgeGroup() const;
		Sex GetSex() const;
		const PersonBehavior& GetBehavior() const;
		const Community& GetCommunity() const;

		const Place* GetWhereabouts() const;
		const Home* GetHome() const;
		const Workplace* GetWorkplace() const;
		const School* GetSchool() const;
		
		void SetWhereabouts(const Place* newWhereabouts);
		void SetHome(Home* newHome);
		void SetWorkplace(const Workplace* newWorkplace);
		void SetSchool(const School* newSchool);
		
		void ChangeBehavior(PersonBehavior newBehavior);
		void Move(Place* destination);

	private:
		void DeseaseCheck();

	private:
		const uint32_t id;
		Age_Group age;
		Sex sex;
		PersonBehavior behavior;
		bool alive = true;

		const Community& community;
		// Not const because we will add ourself to the home
		Home* home;
		const Place* whereabouts;
		const Workplace* workplace = nullptr;
		const School* school = nullptr;

		std::unique_ptr<PersonStates> personState = nullptr;

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

	};
}
