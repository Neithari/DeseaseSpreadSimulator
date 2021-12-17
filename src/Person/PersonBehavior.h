#pragma once
namespace DeseaseSpreadSimulation
{
	struct PersonBehavior
	{
		PersonBehavior(uint16_t foodBuyInterval = 7, uint16_t hardwareBuyInterval = 30);

		// Interval in days
		uint16_t foodBuyInterval;
		uint16_t hardwareBuyInterval;
	};
}
