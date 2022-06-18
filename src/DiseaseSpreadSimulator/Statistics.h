#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include "Enums.h"

namespace DiseaseSpreadSimulation
{
	namespace Statistics
	{
		struct HumanDistribution
		{
			HumanDistribution(Age_Group inAgeGroup, Sex inSex, float inPercent)
				: ageGroup(inAgeGroup),
				  sex(inSex),
				  percent(inPercent){};

			inline bool operator==(const HumanDistribution& rhs) const
			{
				return ageGroup == rhs.ageGroup && sex == rhs.sex;
			};

			Age_Group ageGroup;
			Sex sex;
			float percent;
		};

		struct HouseholdComposition
		{
			// All in percent from 0.0 - 1.0
			constexpr HouseholdComposition(float single, float twoToThree, float fourToFive, float sixPlus)
				: oneMember(single),
				  twoToThreeMembers(twoToThree),
				  fourToFiveMembers(fourToFive),
				  sixPlusMembers(sixPlus){};

			const float oneMember;
			const float twoToThreeMembers;
			const float fourToFiveMembers;
			const float sixPlusMembers;
		};

		// Workplace size estimates https://www.statista.com/statistics/944669/current-office-size-full-time-employees-usa/
		static constexpr std::array<float, 5> workplaceSize{0.2649f, 0.308f, 0.1908f, 0.0821f, 0.1542f};

		// United States Census Bureau - Population by Age and Sex:2018 -> https://www.census.gov/data/tables/2018/demo/age-and-sex/2018-age-sex-composition.html
		// Male population is only 99.9% in that document. Added .1% to AboveEighty to get to 100% and prevent possible bugs
		// Value is in percent with 1.f = 100%
		static const std::vector<HumanDistribution> defaultAgeDistributionUSA{
			{Age_Group::UnderTen, Sex::Female, 0.0595f},
			{Age_Group::UnderTwenty, Sex::Female, 0.062f},
			{Age_Group::UnderThirty, Sex::Female, 0.067f},
			{Age_Group::UnderFourty, Sex::Female, 0.0655f},
			{Age_Group::UnderFifty, Sex::Female, 0.062f},
			{Age_Group::UnderSixty, Sex::Female, 0.0665f},
			{Age_Group::UnderSeventy, Sex::Female, 0.0595f},
			{Age_Group::UnderEighty, Sex::Female, 0.0365f},
			{Age_Group::AboveEighty, Sex::Female, 0.0215f},

			{Age_Group::UnderTen, Sex::Male, 0.0645f},
			{Age_Group::UnderTwenty, Sex::Male, 0.067f},
			{Age_Group::UnderThirty, Sex::Male, 0.071f},
			{Age_Group::UnderFourty, Sex::Male, 0.0675f},
			{Age_Group::UnderFifty, Sex::Male, 0.062f},
			{Age_Group::UnderSixty, Sex::Male, 0.065f},
			{Age_Group::UnderSeventy, Sex::Male, 0.055f},
			{Age_Group::UnderEighty, Sex::Male, 0.0325f},
			{Age_Group::AboveEighty, Sex::Male, 0.0155f}};
		// United Nations - Household Size & Composition, 2019 -> https://population.un.org/Household/index.html#/countries/840
		static constexpr HouseholdComposition householdUSA{0.2789f, 0.4949f, 0.1881f, 0.0381f};
		static constexpr HouseholdComposition householdGermany{0.3953f, 0.47f, 0.1271f, 0.0076f};

		// NCES - Public elementary and secondary school stats -> https://nces.ed.gov/programs/digest/d20/tables/dt20_214.40.asp
		static constexpr uint32_t averageSchoolSizeUSA{527};
		// Baden-Wuertemberg - Sch�ler und Schulen nach Schularten -> https://www.statistik-bw.de/BildungKultur/SchulenAllgem/1301518x.tab?R=LA
		// Average per school - schoolkids / school count rounded
		static constexpr uint32_t averageSchoolSizeGermany{236};

		static constexpr auto AverageSchoolSize(Country country)
		{
			switch (country)
			{
			case Country::USA:
				return Statistics::averageSchoolSizeUSA;
				break;
			case Country::Germany:
				return Statistics::averageSchoolSizeGermany;
				break;
			default:
				return Statistics::averageSchoolSizeUSA;
				break;
			}
		}
	} // namespace Statistics
} // namespace DiseaseSpreadSimulation
