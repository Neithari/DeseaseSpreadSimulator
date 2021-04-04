#pragma once

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
		void SetSimulationTimeMultiplier(unsigned int multiplier);

	private:
		std::chrono::time_point<std::chrono::steady_clock> currentFrameTime;
		std::chrono::time_point<std::chrono::steady_clock> lastFrameTime;
		// milliseconds
		std::chrono::milliseconds frameTime = {};
		uint64_t frameSum = 0;
		// tick in ms
		uint64_t tick = 1000;
		// seconds
		uint64_t simulationTime = 0;

		unsigned int simulationTimeMultiplier = 10;
	};
}
