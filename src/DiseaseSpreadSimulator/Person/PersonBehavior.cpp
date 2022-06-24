#include "Person/PersonBehavior.h"
#include <array>
#include <random>
#include "RandomNumbers.h"

// I can't see a beneficial fix for this
// NOLINTNEXTLINE(*-easily-swappable-parameters)
DiseaseSpreadSimulation::PersonBehavior::PersonBehavior(uint32_t foodBuyIntervalIn, uint32_t hardwareBuyIntervalIn, float acceptanceFactorIn, float travelNeedIn) // NOLINT
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
	static constexpr std::array<float, 7> acceptanceIntervals{0.F, .2F, .6F, .7F, .8F, .9F, 1.F};
	static constexpr std::array<float, 6> acceptanceWeights{1.F, 0.F, 10.F, 10.F, 29.F, 50.F};
	static constexpr std::array<float, 5> travelIntervals{0.F, .0005F, .001F, .005F, .01F};
	static constexpr std::array<float, 4> travelweights{45.F, 30.F, 20.F, 5.F};

	std::piecewise_constant_distribution<float> acceptanceDistribution(acceptanceIntervals.begin(), acceptanceIntervals.end(), acceptanceWeights.begin());
	acceptanceFactor = acceptanceDistribution(Random::generator);
	std::piecewise_constant_distribution<float> travelDistribution(travelIntervals.begin(), travelIntervals.end(), travelweights.begin());
	travelNeed = travelDistribution(Random::generator);
}
