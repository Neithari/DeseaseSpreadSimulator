#pragma once
#include "Simulation/TimeObserver.h"

namespace DeseaseSpreadSimulation
{
	class TimeManager
	{
	public:
		// Singleton instance
		static TimeManager& Instance();

		void Start();
		void Pause();

		// Call once every frame
		void Update();
		// Returns elapsed hours since the start of the simulation.
		uint64_t GetElapsedHours() const;
		// Returns elapsed days since the start of the simulation. Rounded down.
		uint64_t GetElapsedDays() const;
		// Calling without a argument will default to 1x speed
		void SetSimulationTimeMultiplier(uint16_t multiplier = 1u);

		Day GetCurrentDay() const;
		bool IsWorkday() const;
		static bool IsWorkday(const Day day);
		// Get the time in 24h format
		uint16_t GetTime() const;

		// Observer pattern functions
		void AddObserver(TimeObserver* observer);
		void RemoveObserver(TimeObserver* observer);
		void NotifyDayChange();

	private:
		TimeManager();
		Day GetNextDay() const;

	private:
		bool pauseTime = true;

		std::chrono::steady_clock::time_point lastTime;
		// Tick in ms
		static constexpr uint16_t timeStepTick = 41u;
		static constexpr auto timeStep = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(timeStepTick));
		std::chrono::nanoseconds frameSum{};
		
		// In simulation hours. 1 hour per time step
		uint64_t simulationTime = 0u;
		uint32_t simulationTimeMultiplier = 1u;

		Day currentDay = Day::Monday;
		uint16_t dayTime = 0u;
		
		std::vector<TimeObserver*> observers;
		std::mutex observersMutex;
	};
}
