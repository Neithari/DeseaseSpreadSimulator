#include "Person/PersonBehavior.h"
#include <array>
#include <random>
#include "RandomNumbers.h"

DiseaseSpreadSimulation::PersonBehavior::PersonBehavior(uint32_t foodBuyIntervalIn, uint32_t hardwareBuyIntervalIn, float acceptanceFactorIn, float travelNeedIn)
	: foodBuyInterval(foodBuyIntervalIn),
	  hardwareBuyInterval(hardwareBuyIntervalIn),
	  acceptanceFactor(acceptanceFactorIn),
	  travelNeed(travelNeedIn)
{
}

DiseaseSpreadSimulation::PersonBehavior::PersonBehavior()
	: foodBuyInterval(Random::UniformIntRange(minDaysToNextFoodBuy,
	  maxDaysToNextFoodBuy)),
	  hardwareBuyInterval(Random::UniformIntRange(minDaysToNextHardwareBuy, maxDaysToNextHardwareBuy))
{
	// Low percentage of people that don't accept the rules (0-20%), some that are not fully commited (60-80%)
	// and the mass that follows rules more or less sloppy (>80%)
	static constexpr std::array<float, 7> acceptanceIntervals{0.F, .2f, .6f, .7f, .8f, .9f, 1.F};
	static constexpr std::array<float, 6> acceptanceWeights{1.F, 0.F, 10.F, 10.F, 29.F, 50.F};
	static constexpr std::array<float, 5> travelIntervals{0.F, .0005f, .001f, .005f, .01f};
	static constexpr std::array<float, 4> travelweights{45.F, 30.F, 20.F, 5.F};

	std::piecewise_constant_distribution<float> acceptanceDistribution(acceptanceIntervals.begin(), acceptanceIntervals.end(), acceptanceWeights.begin());
	acceptanceFactor = acceptanceDistribution(Random::generator);
	std::piecewise_constant_distribution<float> travelDistribution(travelIntervals.begin(), travelIntervals.end(), travelweights.begin());
	travelNeed = travelDistribution(Random::generator);
}
