#pragma once
#include <cstdint>
#include <string>
#include "Disease/Disease.h"
#include "Enums.h"

namespace DiseaseSpreadSimulation
{
	class Person;
	class Community;

	class Infection
	{
	public:
		Infection() = default;

		void Contaminate(const Disease* infection, Age_Group age);
		[[nodiscard]] const Disease* GetDisease() const;
		void Update(Person& person, bool isNewDay);
		void IncreaseSpreadCount();

		static bool WillInfect(const Infection& exposed, float acceptanceFactor, const Community* community);
		[[nodiscard]] bool IsSusceptible() const;
		[[nodiscard]] bool IsInfectious() const;
		[[nodiscard]] bool IsFatal() const;
		[[nodiscard]] bool HasDisease() const;
		[[nodiscard]] const std::string& GetDiseaseName() const;
		[[nodiscard]] bool HasRecovered() const;
		[[nodiscard]] bool HasSymptoms() const;

		[[nodiscard]] uint32_t GetSpreadCount() const;

	private:
		// Advance daysTillOutbreak, daysContagious, daysTillCured, daysToLive by a delta time
		void AdvanceDay(Person& person);
		void DiseaseCheck();

	
		Seir_State seirState{Seir_State::Susceptible};
		bool hasRecovered{false};
		bool hasSymptoms{false};
		uint32_t spreadCount{0};

		const Disease* disease{nullptr};
		uint32_t latentPeriod{0};
		uint32_t daysInfectious{0};
		uint32_t daysTillCured{0};
		uint32_t daysToLive{0};
		bool isFatal{false};
		float spreadFactor{0.F};

		std::string noDisease{};
	};
} // namespace DiseaseSpreadSimulation
