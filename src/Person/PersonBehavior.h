#pragma once
namespace DeseaseSpreadSimulation
{
	struct PersonBehavior
	{
		constexpr PersonBehavior(uint16_t foodBuyInterval = 7u, uint16_t hardwareBuyInterval = 30u, float acceptanceFactor = 1.f);

		// Interval in days
		uint16_t foodBuyInterval;
		uint16_t hardwareBuyInterval;

		// Percent from 0-1
		float acceptanceFactor;
	};

	static constexpr std::array<PersonBehavior, 3> personBehaviors{
		// No self restriction
		PersonBehavior(2u, 15u, 0.1f),
		// Normal self restriction
		PersonBehavior(7u, 30u, 0.9f),
		// Extended self restriction
		PersonBehavior(14u,60u, 1.f) };
}
