#pragma once
#include <type_traits>

namespace DeseaseSpreadSimulation
{
	class Person;

	class Infection
	{
	public:
		Infection() = default;

		void Contaminate(const Desease* infection, Age_Group age);
		const Desease* GetDesease();
		void Update(Person& person, bool isNewDay);
		void IncreaseSpreadCount();

		bool WillInfect(const Desease* exposed, float acceptanceFactor) const;
		bool IsSusceptible() const;
		bool IsInfectious() const;
		bool IsFatal() const;
		bool HasDesease() const;
		std::string GetDeseaseName() const;
		bool HasRecovered() const;
		bool HasSymptoms() const;

		uint32_t GetSpreadCount() const;
		const Desease* GetDesease() const;

	private:
		// Advance daysTillOutbreak, daysContagious, daysTillCured, daysToLive by a delta time
		void AdvanceDay(Person& person);
		void DeseaseCheck();

	private:
		Seir_State seirState = Seir_State::Susceptible;
		bool hasRecovered = false;
		bool hasSymptoms = false;
		uint32_t spreadCount = 0;

		const Desease* desease = nullptr;
		uint16_t latentPeriod = 0;
		uint16_t daysInfectious = 0;
		uint16_t daysTillCured = 0;
		uint16_t daysToLive = 0;
		bool isFatal = false;
	};
}
