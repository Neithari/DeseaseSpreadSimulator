#pragma once
#include "Simulation/TimeObserver.h"

namespace DeseaseSpreadSimulation
{
	class TimeManager
	{
	public:
		TimeManager();
		// call once every frame
		void Update();
		// in milliseconds
		int64_t GetFrameTime() const;
		// returns the simulation days elapsed since the start of the simulation. Rounded down to full days.
		unsigned long long GetElapsedDays() const;
		// calling without a argument will default to 1x speed
		void SetSimulationTimeMultiplier(uint16_t multiplier = 1u);

		Day GetCurrentDay() const;

		// Observer pattern functions
		void AddObserver(TimeObserver* observer);
		void RemoveObserver(TimeObserver* observer);
		void NotifyDayChange();

	private:
		Day GetNextDay() const;

	private:
		std::chrono::time_point<std::chrono::steady_clock> currentFrameTime;
		std::chrono::time_point<std::chrono::steady_clock> lastFrameTime;
		// milliseconds
		std::chrono::milliseconds frameTime = {};
		uint32_t frameSum = 0u;
		// tick in ms
		uint32_t tick = 1000u;
		// in simulation days. 1 day per tick
		uint64_t simulationTime = 0u;
		uint32_t simulationTimeMultiplier = 1u;
		Day currentDay = Day::Monday;
		
		std::vector<TimeObserver*> observers;
	};
}
