#pragma once
#include <type_traits>

namespace DiseaseSpreadSimulation
{
	class Person;

	class Infection
	{
	public:
		Infection() = default;

		void Contaminate(const Disease* infection, Age_Group age);
		const Disease* GetDisease();
		void Update(Person& person, bool isNewDay);
		void IncreaseSpreadCount();

		bool WillInfect(const Infection& exposed, float acceptanceFactor) const;
		bool IsSusceptible() const;
		bool IsInfectious() const;
		bool IsFatal() const;
		bool HasDisease() const;
		std::string GetDiseaseName() const;
		bool HasRecovered() const;
		bool HasSymptoms() const;

		uint32_t GetSpreadCount() const;
		const Disease* GetDisease() const;

	private:
		// Advance daysTillOutbreak, daysContagious, daysTillCured, daysToLive by a delta time
		void AdvanceDay(Person& person);
		void DiseaseCheck();

	private:
		Seir_State seirState = Seir_State::Susceptible;
		bool hasRecovered = false;
		bool hasSymptoms = false;
		uint32_t spreadCount = 0;

		const Disease* disease = nullptr;
		uint16_t latentPeriod = 0;
		uint16_t daysInfectious = 0;
		uint16_t daysTillCured = 0;
		uint16_t daysToLive = 0;
		bool isFatal = false;
		float spreadFactor = 0.f;
	};
}
