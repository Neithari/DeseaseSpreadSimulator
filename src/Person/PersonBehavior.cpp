#include "pch.h"
#include "Person/PersonBehavior.h"

constexpr DeseaseSpreadSimulation::PersonBehavior::PersonBehavior(uint16_t foodBuyInterval, uint16_t hardwareBuyInterval, float acceptanceFactor)
	:
	foodBuyInterval(foodBuyInterval),
	hardwareBuyInterval(hardwareBuyInterval),
	acceptanceFactor(acceptanceFactor)
{
}

DeseaseSpreadSimulation::PersonBehavior::PersonBehavior()
{
	std::random_device seed;
	std::mt19937 generator(seed());

	std::uniform_int_distribution foodBuyDistribution(minDaysToNextFoodBuy, maxDaysToNextFoodBuy);
	foodBuyInterval = foodBuyDistribution(generator);

	std::uniform_int_distribution hardwareBuyDistribution(minDaysToNextHardwareBuy, maxDaysToNextHardwareBuy);
	hardwareBuyInterval = hardwareBuyDistribution(generator);

	// Low percentage of people that don't accept the rules (0-20%), some that are sloppy (60-80%) and the mass that follows rules (>80%)
	std::array<float, 7> intervals{ 0.f, .2f, .6f, .7f, .8f, .9f, 1.f };
	std::array<float, 6> weights{ 1.f, 0.f, 10.f, 10.f, 29.f, 50.f };
	std::piecewise_constant_distribution<float> acceptanceDistribution(intervals.begin(), intervals.end(), weights.begin());
	acceptanceFactor = acceptanceDistribution(generator);
}
