#pragma once
#include <cstdint>

namespace DiseaseSpreadSimulation
{
	struct PersonBehavior
	{
		// Intervals in days, acceptance in percent
		PersonBehavior(uint32_t foodBuyIntervalIn, uint32_t hardwareBuyIntervalIn, float acceptanceFactorIn, float travelNeedIn);
		// Construct a randomized behavior
		PersonBehavior();

		// Interval in days
		uint32_t foodBuyInterval;
		uint32_t hardwareBuyInterval;

		// Percent from 0-1
		float acceptanceFactor;
		float travelNeed;

	private:
		static constexpr uint32_t minDaysToNextFoodBuy = 1u;
		static constexpr uint32_t maxDaysToNextFoodBuy = 30u;
		static constexpr uint32_t minDaysToNextHardwareBuy = 10u;
		static constexpr uint32_t maxDaysToNextHardwareBuy = 90u;
	};
} // namespace DiseaseSpreadSimulation
