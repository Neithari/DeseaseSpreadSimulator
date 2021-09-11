#pragma once
#include "Places/Places.h"

namespace DeseaseSpreadSimulation
{
	enum class Country {USA,Germany};
	
	class PersonPopulator
	{
	public:
		struct HumanDistribution
		{
			HumanDistribution(Age_Group ageGroup, Sex sex, float percent)
				:
				ageGroup(ageGroup),
				sex(sex),
				percent(percent)
			{};
			

			inline bool operator==(const HumanDistribution& rhs)
			{
				return ageGroup == rhs.ageGroup && sex == rhs.sex;
			};

			Age_Group ageGroup = Age_Group::UnderTen;
			Sex sex = Sex::Female;
			float percent = 0.f;
		};

		struct HouseholdComposition
		{
			// All in percent from 0.0 - 1.0
			HouseholdComposition(float oneMember, float twoToThreeMembers, float fourToFiveMembers, float sixPlusMembers)
				:
				oneMember(oneMember),
				twoToThreeMembers(twoToThreeMembers),
				fourToFiveMembers(fourToFiveMembers),
				sixPlusMembers(sixPlusMembers)
			{};

			float oneMember = 0.f;
			float twoToThreeMembers = 0.f;
			float fourToFiveMembers = 0.f;
			float sixPlusMembers = 0.f;
		};
		
		PersonPopulator(size_t populationSize, std::vector<HumanDistribution> distribution);

		// Get a new person for the chosen distribution. Call should return a empty pointer when population size is reached
		std::unique_ptr<Person> GetNewPerson(Home* home);

		// Helper function to select the distribution inside the constructor initializer list
		static std::vector<HumanDistribution> GetCountryDistribution(Country country);

	private:
		// Returns a rounded down percentage of count
		static size_t DistributionToCountHelper(size_t count, float percent);

	private:
		const size_t populationSize = 0;
		size_t leftover = 0;
		bool lastFew = false;
		const std::vector<HumanDistribution> ageDistribution;
		// Set to 1 because we use the distribution on 0 inside the constructor
		size_t ageDistributionIndex = 1;
		HumanDistribution currentHumanDistribution;
		size_t currentHumanCount = 0;

		/// TODO: Change the static data to something better
		// United States Census Bureau - Population by Age and Sex:2018 -> https://www.census.gov/data/tables/2018/demo/age-and-sex/2018-age-sex-composition.html
		// Male population is only 99.9% in that document. Added .1% to AboveEighty to get to 100% and prevent possible bugs
		static const std::vector<HumanDistribution> defaultAgeDistributionUSA;

		// United Nations - Household Size & Composition, 2019 -> https://population.un.org/Household/index.html#/countries/840
		static const HouseholdComposition householdUSA;
		static const HouseholdComposition householdGermany;
	};
}
