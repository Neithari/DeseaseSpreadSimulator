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
		
		// In simulation hours. 1 hour per time step
		uint64_t simulationTime = 0u;
		uint16_t simulationTimeMultiplier = 1u;

		Day currentDay = Day::Monday;
		bool isWorkday = true;
		uint16_t dayTime = 0u;
		
		std::vector<TimeObserver*> observers;
		std::mutex observersMutex;
	};
}
