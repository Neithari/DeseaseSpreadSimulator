#pragma once

namespace DeseaseSpreadSimulation
{
	class Person;

	class PersonPopulator
	{
	public:
		struct Human
		{
			Human(Age_Group ageGroup, Sex sex, float percent)
				:
				ageGroup(ageGroup),
				sex(sex),
				percent(percent)
			{};

			inline bool operator==(const Human& rhs)
			{
				return ageGroup == rhs.ageGroup && sex == rhs.sex;
			};

			Age_Group ageGroup = Age_Group::UnderTen;
			Sex sex = Sex::Female;
			float percent = 0.f;
		};

		static std::vector<Person> GetPopulation(size_t count, const std::vector<Human>& distribution = defaultAgeDistributionUSA);

	private:
		PersonPopulator() = default;
		
		// Returns a rounded down percentage of count
		static size_t DistributionToCountHelper(size_t count, float percent);

	private:
		// United States Census Bureau - Population by Age and Sex:2018 -> https://www.census.gov/data/tables/2018/demo/age-and-sex/2018-age-sex-composition.html
		// Male population is only 99.9% in that document. Added .1% to AboveEighty to get to 100% and prevent possible bugs
		static const std::vector<Human> defaultAgeDistributionUSA;
	};
}
