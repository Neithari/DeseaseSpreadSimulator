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

	static constexpr std::array<PersonBehavior, 3> personBehaviors{
		// No self restriction
		PersonBehavior(2u, 15u, 0.1f),
		// Normal self restriction
		PersonBehavior(7u, 30u, 0.9f),
		// Extended self restriction
		PersonBehavior(14u,60u, 1.f) };
}
