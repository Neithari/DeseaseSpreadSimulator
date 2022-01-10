#pragma once
namespace DeseaseSpreadSimulation
{
	struct PersonBehavior
	{
		// Intervals in days, acceptance in percent
		constexpr PersonBehavior(uint16_t foodBuyInterval, uint16_t hardwareBuyInterval, float acceptanceFactor);
		// Construct a randomized behavior
		PersonBehavior();

		// Interval in days
		uint16_t foodBuyInterval;
		uint16_t hardwareBuyInterval;

		// Percent from 0-1
		float acceptanceFactor;

	private:
		static constexpr uint16_t minDaysToNextFoodBuy = 1u;
		static constexpr uint16_t maxDaysToNextFoodBuy = 30u;
		static constexpr uint16_t minDaysToNextHardwareBuy = 10u;
		static constexpr uint16_t maxDaysToNextHardwareBuy = 90u;
	};
}
