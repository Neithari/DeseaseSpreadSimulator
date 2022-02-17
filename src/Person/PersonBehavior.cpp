#include "pch.h"
#include "Person/PersonBehavior.h"

DeseaseSpreadSimulation::PersonBehavior::PersonBehavior(uint16_t foodBuyInterval, uint16_t hardwareBuyInterval, float acceptanceFactor, float travelNeed)
	:
	foodBuyInterval(foodBuyInterval),
	hardwareBuyInterval(hardwareBuyInterval),
	acceptanceFactor(acceptanceFactor),
	travelNeed(travelNeed)
{
}

DeseaseSpreadSimulation::PersonBehavior::PersonBehavior()
{
	foodBuyInterval = Random::UniformIntRange(minDaysToNextFoodBuy, maxDaysToNextFoodBuy);
	hardwareBuyInterval = Random::UniformIntRange(minDaysToNextHardwareBuy, maxDaysToNextHardwareBuy);

	// Low percentage of people that don't accept the rules (0-20%), some that are not fully commited (60-80%)
	// and the mass that follows rules more or less sloppy (>80%)
	static constexpr std::array<float, 7> acceptanceIntervals{ 0.f, .2f, .6f, .7f, .8f, .9f, 1.f };
	static constexpr std::array<float, 6> acceptanceWeights{ 1.f, 0.f, 10.f, 10.f, 29.f, 50.f };
	static constexpr std::array<float, 5> travelIntervals{ 0.f, .0005f, .001f, .005f, .01f };
	static constexpr std::array<float, 4> travelweights{ 45.f, 30.f, 20.f, 5.f};
	
	std::piecewise_constant_distribution<float> acceptanceDistribution(acceptanceIntervals.begin(), acceptanceIntervals.end(), acceptanceWeights.begin());
	acceptanceFactor = acceptanceDistribution(Random::generator);
	std::piecewise_constant_distribution<float> travelDistribution(travelIntervals.begin(), travelIntervals.end(), travelweights.begin());
	travelNeed = travelDistribution(Random::generator);
}
