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
		// In milliseconds
		int64_t GetFrameTime() const;
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

		std::chrono::time_point<std::chrono::steady_clock> currentFrameTime;
		std::chrono::time_point<std::chrono::steady_clock> lastFrameTime;
		std::chrono::milliseconds frameTime = {};
		// Tick in ms
		uint32_t tick = 50u;
		uint32_t frameSum = 0u;
		// In simulation hours. 1 hour per tick
		uint64_t simulationTime = 0u;
		uint32_t simulationTimeMultiplier = 1u;

		Day currentDay = Day::Monday;
		uint16_t dayTime = 0u;
		
		std::vector<TimeObserver*> observers;
		std::mutex observersMutex;
	};
}
