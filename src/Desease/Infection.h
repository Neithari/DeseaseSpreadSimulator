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

		uint32_t GetSpreadCount() const;

	private:
		// Advance daysTillOutbreak, daysContagious, daysTillCured, daysToLive by a delta time
		void AdvanceDay(Person& person);
		void DeseaseCheck();

		template <typename T, typename = typename std::enable_if_t<std::is_floating_point<T>::value>>
		T MapOneRangeToAnother(T value, T fromRangeMin, T fromRangeMax, T toRangeMin, T toRangeMax) const
		{
			return toRangeMin + (((value - fromRangeMin) * (toRangeMax - toRangeMin)) / (fromRangeMax - fromRangeMin));
		}

	private:
		Seir_State seirState = Seir_State::Susceptible;
		bool hasRecovered = false;
		uint32_t spreadCount = 0;

		const Desease* desease = nullptr;
		uint16_t latentPeriod = 0;
		uint16_t daysInfectious = 0;
		uint16_t daysTillCured = 0;
		uint16_t daysToLive = 0;
		bool isFatal = false;
	};
}
