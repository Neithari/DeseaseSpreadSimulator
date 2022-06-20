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
		static constexpr std::array<float, 5> workplaceSize{0.2649F, 0.308F, 0.1908F, 0.0821F, 0.1542F};

		// United States Census Bureau - Population by Age and Sex:2018 -> https://www.census.gov/data/tables/2018/demo/age-and-sex/2018-age-sex-composition.html
		// Male population is only 99.9% in that document. Added .1% to AboveEighty to get to 100% and prevent possible bugs
		// Value is in percent with 1.f = 100%
		static const std::vector<HumanDistribution> defaultAgeDistributionUSA{
			{Age_Group::UnderTen, Sex::Female, 0.0595F},
			{Age_Group::UnderTwenty, Sex::Female, 0.062F},
			{Age_Group::UnderThirty, Sex::Female, 0.067F},
			{Age_Group::UnderFourty, Sex::Female, 0.0655F},
			{Age_Group::UnderFifty, Sex::Female, 0.062F},
			{Age_Group::UnderSixty, Sex::Female, 0.0665F},
			{Age_Group::UnderSeventy, Sex::Female, 0.0595F},
			{Age_Group::UnderEighty, Sex::Female, 0.0365F},
			{Age_Group::AboveEighty, Sex::Female, 0.0215F},

			{Age_Group::UnderTen, Sex::Male, 0.0645F},
			{Age_Group::UnderTwenty, Sex::Male, 0.067F},
			{Age_Group::UnderThirty, Sex::Male, 0.071F},
			{Age_Group::UnderFourty, Sex::Male, 0.0675F},
			{Age_Group::UnderFifty, Sex::Male, 0.062F},
			{Age_Group::UnderSixty, Sex::Male, 0.065F},
			{Age_Group::UnderSeventy, Sex::Male, 0.055F},
			{Age_Group::UnderEighty, Sex::Male, 0.0325F},
			{Age_Group::AboveEighty, Sex::Male, 0.0155F}};
		// United Nations - Household Size & Composition, 2019 -> https://population.un.org/Household/index.html#/countries/840
		static constexpr HouseholdComposition householdUSA{0.2789F, 0.4949F, 0.1881F, 0.0381F};
		static constexpr HouseholdComposition householdGermany{0.3953F, 0.47F, 0.1271F, 0.0076F};

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
