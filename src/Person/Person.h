#pragma once
#include "Places/Places.h"

namespace DeseaseSpreadSimulation
{
	class Person
	{
	public:
		// Create a Person with age, sex, set it's home and set whereabout to home
		Person(Age_Group age, Sex sex, Home* home = nullptr);
		
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

		bool isSusceptible() const;
		bool isInfectious() const;
		bool isQuarantined() const;
		bool isAlive() const;
		bool hasDesease(const std::string& deseaseName) const;

		uint32_t GetID() const;
		Age_Group GetAgeGroup() const;
		Sex GetSex() const;
		auto GetWhereabouts() const;
		auto GetHome() const;
		void SetWorkplace(Place* newWorkplace);


	private:
		void Move();
		void DeseaseCheck();

	private:
		const uint32_t id;
		Age_Group age;
		Sex sex;

		Home* home;
		Place* whereabouts;
		Place* workplace = nullptr;

		Seir_State state = Seir_State::Susceptible;
		bool quarantined = false;
		unsigned int spreadCount = 0;

		// Desease Stuff
		const Desease* desease = nullptr;
		unsigned int latentPeriod = 0;
		unsigned int daysInfectious = 0;
		unsigned int daysTillCured = 0;

		bool alive = true;
		unsigned int daysToLive = 0;
		bool willDie = false;
	};
}
