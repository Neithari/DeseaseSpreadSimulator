#include "pch.h"
#include "Person/PersonPopulator.h"

// Value is in percent with 1.f = 100%
const std::vector<DeseaseSpreadSimulation::PersonPopulator::HumanDistribution> DeseaseSpreadSimulation::PersonPopulator::defaultAgeDistributionUSA{
	{ Age_Group::UnderTen,	  Sex::Female, 0.119f },
	{ Age_Group::UnderTwenty, Sex::Female, 0.124f },
	{ Age_Group::UnderThirty, Sex::Female, 0.134f },
	{ Age_Group::UnderFourty, Sex::Female, 0.131f },
	{ Age_Group::UnderFifty,  Sex::Female, 0.124f },
	{ Age_Group::UnderSixty,  Sex::Female, 0.133f },
	{ Age_Group::UnderSeventy,Sex::Female, 0.119f },
	{ Age_Group::UnderEighty, Sex::Female, 0.073f },
	{ Age_Group::AboveEighty, Sex::Female, 0.043f },

	{ Age_Group::UnderTen,	  Sex::Male,   0.129f },
	{ Age_Group::UnderTwenty, Sex::Male,   0.134f },
	{ Age_Group::UnderThirty, Sex::Male,   0.142f },
	{ Age_Group::UnderFourty, Sex::Male,   0.135f },
	{ Age_Group::UnderFifty,  Sex::Male,   0.124f },
	{ Age_Group::UnderSixty,  Sex::Male,   0.130f },
	{ Age_Group::UnderSeventy,Sex::Male,   0.110f },
	{ Age_Group::UnderEighty, Sex::Male,   0.065f },
	{ Age_Group::AboveEighty, Sex::Male,   0.031f }
};

const DeseaseSpreadSimulation::PersonPopulator::HouseholdComposition DeseaseSpreadSimulation::PersonPopulator::householdUSA{27.89f, 49.49f, 18.81f, 3.81f};
const DeseaseSpreadSimulation::PersonPopulator::HouseholdComposition DeseaseSpreadSimulation::PersonPopulator::householdGermany{39.53f, 47.0f, 12.71f, 0.76f};

DeseaseSpreadSimulation::PersonPopulator::PersonPopulator(size_t populationSize, std::vector<HumanDistribution> distribution)
	:
	populationSize(populationSize),
	leftover(populationSize),
	ageDistribution(std::move(distribution)),
	currentHumanDistribution(ageDistribution.front())
{
	// Set the currentHumanCount to a percent of the population and to 1 if this will return 0
	currentHumanCount = DistributionToCountHelper(populationSize, currentHumanDistribution.percent);
	if (currentHumanCount == 0)
	{
		currentHumanCount = 1;
	}
}

std::unique_ptr<DeseaseSpreadSimulation::Person> DeseaseSpreadSimulation::PersonPopulator::GetNewPerson(std::shared_ptr<DeseaseSpreadSimulation::Home> home)
{
	// As long as we don't have assigned the full population create a new person with age and sex according to our distribution
	if (leftover > 0)
	{
		// When the currentHumanCount is 0...
		if (currentHumanCount == 0)
		{
			// ...we set the new current distribution...
			currentHumanDistribution = ageDistribution.at(ageDistributionIndex);
			
			if (!lastFew)
			{
				// ...set the currentHumanCount to a percent of the population...
				currentHumanCount = DistributionToCountHelper(populationSize, currentHumanDistribution.percent);
			}
			// ...and to 1 if this will return 0...
			if (currentHumanCount == 0)
			{
				currentHumanCount = 1;
			}
			
			// ...advance the index and check if we used all distributions...
			if (++ageDistributionIndex >= ageDistribution.size())
			{
				// ...if yes we start again at the beginning...
				ageDistributionIndex = 0;
				// ...and distribute 1 to every human distribution untill no leftover
				lastFew = true;
			}
		}

		leftover--;
		currentHumanCount--;

		return std::make_unique<Person>(currentHumanDistribution.ageGroup, currentHumanDistribution.sex, home);
	}

	return nullptr;
}

size_t DeseaseSpreadSimulation::PersonPopulator::DistributionToCountHelper(size_t count, float percent)
{
	// Scale count by percent and then omit the decimal
	return static_cast<size_t>(count * static_cast<double>(percent));
}

std::vector<DeseaseSpreadSimulation::PersonPopulator::HumanDistribution> DeseaseSpreadSimulation::PersonPopulator::GetCurrentDistribution(Country country)
{
	switch (country)
	{
	case DeseaseSpreadSimulation::Country::USA:
		return defaultAgeDistributionUSA;
		break;
	case DeseaseSpreadSimulation::Country::Germany:
		return defaultAgeDistributionUSA;
		break;
	default:
		return defaultAgeDistributionUSA;
		break;
	}
}
