#pragma once
#include <cstdint>
#include "Enums.h"

namespace DiseaseSpreadSimulation
{
	class TimeManager
	{
	public:
		TimeManager() = default;

		// Call once every frame. Will advance 1 hour per call.
		void Update();
		// Returns elapsed hours since the start of the simulation.
		uint64_t GetElapsedHours() const;
		// Returns elapsed days since the start of the simulation. Rounded down.
		uint64_t GetElapsedDays() const;

		Day GetCurrentDay() const;
		bool IsWorkday() const;
		// Get the time in 24h format
		uint32_t GetTime() const;

	private:
		Day GetNextDay() const;

	private:
		// In simulation hours. 1 hour per update
		uint64_t simulationTime{0U};
		Day currentDay{Day::Monday};
		bool isWorkday{true};
		uint32_t dayTime{0u};
		static constexpr uint32_t hoursPerDay{24U};
	};
} // namespace DiseaseSpreadSimulation
