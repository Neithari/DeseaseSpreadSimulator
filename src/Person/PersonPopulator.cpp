#include "pch.h"
#include "Person/PersonPopulator.h"
#include "Person/Person.h"

// Value is in percent with 1.f = 100%
const std::vector<DeseaseSpreadSimulation::PersonPopulator::Human> DeseaseSpreadSimulation::PersonPopulator::defaultAgeDistributionUSA{
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

std::vector<DeseaseSpreadSimulation::Person> DeseaseSpreadSimulation::PersonPopulator::GetPopulation(size_t count, const std::vector<Human>& distribution)
{
	std::vector<Person> population;
	population.reserve(count);
	size_t leftover = count;

	// As long as we don't have assigned the full population...
	while (leftover > 0)
	{
		count = leftover;
		// ...create a new person with age and sex according to our distribution
		for (auto const& human : distribution)
		{
			if (leftover == 0)
			{
				break;
			}
			/// TODO: Implement a positioning algorithm
			std::pair<float, float> position{ 0.f,0.f };

			size_t dist = DistributionToCountHelper(count, human.percent);
			if (dist == 0 && count > 0)
			{
				dist = 1;
			}
			for (size_t i = 0; i < dist; i++)
			{
				population.emplace_back(human.ageGroup, human.sex, position);
			}
			leftover -= dist;
		}
	}

	return std::move(population);
}

size_t DeseaseSpreadSimulation::PersonPopulator::DistributionToCountHelper(size_t count, float percent)
{
	return static_cast<size_t>(count * percent);
}
