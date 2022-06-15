#include <gtest/gtest.h>
#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include "Enums.h"
#include "Disease/Disease.h"
#include "Disease/DiseaseBuilder.h"

namespace UnitTests
{
	template <typename T>
	::testing::AssertionResult IsBetweenInclusive(T val, T a, T b)
	{
		if ((val >= a) && (val <= b))
			return ::testing::AssertionSuccess();
		else
			return ::testing::AssertionFailure()
				   << val << " is outside the range " << a << " to " << b;
	}

	class DiseaseTest : public ::testing::Test
	{
	protected:
		std::string name = "a";
		std::pair<uint32_t, uint32_t> incubationPeriod{1u, 1u};
		uint32_t daysInfectious = 1;
		std::pair<uint32_t, uint32_t> diseaseDurationRange{2u, 10u};
		std::vector<float> mortalityByAge{1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f};
		std::pair<uint32_t, uint32_t> daysTillDeathRange{1u, 2u};
		std::pair<float, float> spreadFactor{1.f, 1.f};
		float testAccuracy{1.0f};
		std::pair<float, float> symptomsDevelopment{1.f, 1.f};
		DiseaseSpreadSimulation::Disease disease{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};

		// Helper function to check if the given x is within the range
		bool inRange(uint32_t x, std::pair<uint32_t, uint32_t> range)
		{
			return x >= range.first && x <= range.second;
		}
	};

	TEST_F(DiseaseTest, TrivialDiseaseReturns)
	{
		EXPECT_EQ(disease.GetDiseaseName(), name);
		EXPECT_TRUE(IsBetweenInclusive(disease.IncubationPeriod(), incubationPeriod.first, incubationPeriod.second));
		EXPECT_EQ(disease.DaysInfectious(), daysInfectious);
	}
	TEST_F(DiseaseTest, DiseaseDurationRange)
	{
		// Check 10 times to get an average
		uint32_t duration = 0;
		for (int i = 0; i < 10; i++)
		{
			duration = disease.GetDiseaseDuration();
			ASSERT_TRUE(inRange(duration, diseaseDurationRange)) << "Disease duration: " << duration << " is not between: " << diseaseDurationRange.first << " and " << diseaseDurationRange.second;
		}
	}
	TEST_F(DiseaseTest, DaysTillDeathRange)
	{
		// Check 10 times to get an average
		uint32_t days = 0;
		for (int i = 0; i < 10; i++)
		{
			days = disease.DaysTillDeath();
			ASSERT_TRUE(inRange(days, daysTillDeathRange)) << "Days till death: " << days << " is not between: " << daysTillDeathRange.first << " and " << daysTillDeathRange.second;
		}
	}
	TEST_F(DiseaseTest, MortalityByAgeGroup)
	{
		ASSERT_EQ(disease.GetMortalityByAgeGroup(DiseaseSpreadSimulation::Age_Group::UnderTen), mortalityByAge.at(0));
		ASSERT_EQ(disease.GetMortalityByAgeGroup(DiseaseSpreadSimulation::Age_Group::UnderTwenty), mortalityByAge.at(1));
		ASSERT_EQ(disease.GetMortalityByAgeGroup(DiseaseSpreadSimulation::Age_Group::UnderThirty), mortalityByAge.at(2));
		ASSERT_EQ(disease.GetMortalityByAgeGroup(DiseaseSpreadSimulation::Age_Group::UnderFourty), mortalityByAge.at(3));
		ASSERT_EQ(disease.GetMortalityByAgeGroup(DiseaseSpreadSimulation::Age_Group::UnderFifty), mortalityByAge.at(4));
		ASSERT_EQ(disease.GetMortalityByAgeGroup(DiseaseSpreadSimulation::Age_Group::UnderSixty), mortalityByAge.at(5));
		ASSERT_EQ(disease.GetMortalityByAgeGroup(DiseaseSpreadSimulation::Age_Group::UnderSeventy), mortalityByAge.at(6));
		ASSERT_EQ(disease.GetMortalityByAgeGroup(DiseaseSpreadSimulation::Age_Group::UnderEighty), mortalityByAge.at(7));
		ASSERT_EQ(disease.GetMortalityByAgeGroup(DiseaseSpreadSimulation::Age_Group::AboveEighty), mortalityByAge.at(8));
	}
	TEST_F(DiseaseTest, MortalityByAge)
	{
		// Check age 0-89
		uint32_t age = 0u;
		for (uint32_t index = 0u; index < mortalityByAge.size(); index++)
		{
			for (uint32_t i = 0u; i < 10u; i++)
			{
				// Age will be between 0 and 89
				age = index * 10u + i;
				// mortalityByAge vector does only have 9 members so prevent an out of bound
				if (index <= 8u)
				{
					EXPECT_FLOAT_EQ(disease.GetMortalityByAge(age), mortalityByAge.at(index));
				}
				else
				{
					EXPECT_FLOAT_EQ(disease.GetMortalityByAge(age), mortalityByAge.back());
				}
			}
		}
		// Check age >=90
		for (age = 90; age < 111; age++)
		{
			EXPECT_FLOAT_EQ(disease.GetMortalityByAge(age), mortalityByAge.back());
		}
	}
	TEST_F(DiseaseTest, EqualsOperator)
	{
		ASSERT_TRUE(disease == disease);

		DiseaseSpreadSimulation::Disease disease1{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		ASSERT_FALSE(disease == disease1);
	}
	TEST_F(DiseaseTest, IsSame)
	{
		ASSERT_TRUE(disease.isSame(disease));

		std::string name1 = "b";
		DiseaseSpreadSimulation::Disease disease1{name1, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease1.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease1));

		std::pair<uint32_t, uint32_t> incubationPeriod1{2, 2};
		DiseaseSpreadSimulation::Disease disease3{name, incubationPeriod1, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease3.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease3));

		uint32_t daysInfectious1 = 2;
		DiseaseSpreadSimulation::Disease disease4{name, incubationPeriod, daysInfectious1, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease4.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease4));

		std::pair<uint32_t, uint32_t> diseaseDurationRange1{0, 11};
		DiseaseSpreadSimulation::Disease disease5{name, incubationPeriod, daysInfectious, diseaseDurationRange1, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease5.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease5));

		std::vector<float> mortalityByAge1{0.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f};
		DiseaseSpreadSimulation::Disease disease6{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge1, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease6.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease6));

		std::pair<uint32_t, uint32_t> daysTillDeathRange1{0, 3};
		DiseaseSpreadSimulation::Disease disease7{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange1, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease7.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease7));

		std::pair<float, float> spreadFactor1{2.f, 2.f};
		DiseaseSpreadSimulation::Disease disease8{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor1, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease8.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease8));

		float testAccuracy1{2.0f};
		DiseaseSpreadSimulation::Disease disease9{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy1, symptomsDevelopment};
		EXPECT_FALSE(disease9.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease9));

		std::pair<float, float> symptomsDevelopment1{2.f, 2.f};
		DiseaseSpreadSimulation::Disease disease10{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment1};
		EXPECT_FALSE(disease10.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease10));
	}

	class DiseaseBuilderTest : public ::testing::Test
	{
	protected:
		std::string name = "a";
		std::pair<uint32_t, uint32_t> incubationPeriod{1u, 1u};
		uint32_t daysInfectious = 1;
		std::pair<uint32_t, uint32_t> diseaseDurationRange{2u, 10u};
		std::vector<float> mortalityByAge{1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f};
		std::pair<uint32_t, uint32_t> daysTillDeathRange{1u, 2u};
		std::pair<float, float> spreadFactor{1.f, 1.f};
		float testAccuracy{1.0f};
		std::pair<float, float> symptomsDevelopment{1.f, 1.f};

		DiseaseSpreadSimulation::Disease disease{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
	};
	// Tests that CreateDisease() asserts are working properly for every set function
	TEST_F(DiseaseBuilderTest, ThrowNameNotSet)
	{
		DiseaseSpreadSimulation::DiseaseBuilder builder;
		//builder.SetDiseaseName(name);
		builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		builder.SetDaysInfectious(daysInfectious);
		builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		builder.SetMortalityByAge(mortalityByAge);
		builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		builder.SetTestAccuracy(testAccuracy);
		builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		ASSERT_THROW(builder.CreateDisease(), std::logic_error);
	}
	TEST_F(DiseaseBuilderTest, ThrowIncubationNotSet)
	{
		DiseaseSpreadSimulation::DiseaseBuilder builder;
		builder.SetDiseaseName(name);
		//builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		builder.SetDaysInfectious(daysInfectious);
		builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		builder.SetMortalityByAge(mortalityByAge);
		builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		builder.SetTestAccuracy(testAccuracy);
		builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		ASSERT_THROW(builder.CreateDisease(), std::logic_error);
	}
	TEST_F(DiseaseBuilderTest, ThrowDaysInfectiousNotSet)
	{
		DiseaseSpreadSimulation::DiseaseBuilder builder;
		builder.SetDiseaseName(name);
		builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		//builder.SetDaysInfectious(daysInfectious);
		builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		builder.SetMortalityByAge(mortalityByAge);
		builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		builder.SetTestAccuracy(testAccuracy);
		builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		ASSERT_THROW(builder.CreateDisease(), std::logic_error);
	}
	TEST_F(DiseaseBuilderTest, ThrowDiseaseDurationNotSet)
	{
		DiseaseSpreadSimulation::DiseaseBuilder builder;
		builder.SetDiseaseName(name);
		builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		builder.SetDaysInfectious(daysInfectious);
		//builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		builder.SetMortalityByAge(mortalityByAge);
		builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		builder.SetTestAccuracy(testAccuracy);
		builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		ASSERT_THROW(builder.CreateDisease(), std::logic_error);
	}
	TEST_F(DiseaseBuilderTest, ThrowMortalityNotSet)
	{
		DiseaseSpreadSimulation::DiseaseBuilder builder;
		builder.SetDiseaseName(name);
		builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		builder.SetDaysInfectious(daysInfectious);
		builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		//builder.SetMortalityByAge(mortalityByAge);
		builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		builder.SetTestAccuracy(testAccuracy);
		builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		ASSERT_THROW(builder.CreateDisease(), std::logic_error);
	}
	TEST_F(DiseaseBuilderTest, ThrowDaysTillDeathNotSet)
	{
		DiseaseSpreadSimulation::DiseaseBuilder builder;
		builder.SetDiseaseName(name);
		builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		builder.SetDaysInfectious(daysInfectious);
		builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		builder.SetMortalityByAge(mortalityByAge);
		//builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		builder.SetTestAccuracy(testAccuracy);
		builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		ASSERT_THROW(builder.CreateDisease(), std::logic_error);
	}
	TEST_F(DiseaseBuilderTest, ThrowSpreadFactorNotSet)
	{
		DiseaseSpreadSimulation::DiseaseBuilder builder;
		builder.SetDiseaseName(name);
		builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		builder.SetDaysInfectious(daysInfectious);
		builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		builder.SetMortalityByAge(mortalityByAge);
		builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		//builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		builder.SetTestAccuracy(testAccuracy);
		builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		ASSERT_THROW(builder.CreateDisease(), std::logic_error);
	}
	TEST_F(DiseaseBuilderTest, ThrowTestAccuracyNotSet)
	{
		DiseaseSpreadSimulation::DiseaseBuilder builder;
		builder.SetDiseaseName(name);
		builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		builder.SetDaysInfectious(daysInfectious);
		builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		builder.SetMortalityByAge(mortalityByAge);
		builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		//builder.SetTestAccuracy(testAccuracy);
		builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		ASSERT_THROW(builder.CreateDisease(), std::logic_error);
	}
	TEST_F(DiseaseBuilderTest, ThrowSymptomsDevelopmentNotSet)
	{
		DiseaseSpreadSimulation::DiseaseBuilder builder;
		builder.SetDiseaseName(name);
		builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		builder.SetDaysInfectious(daysInfectious);
		builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		builder.SetMortalityByAge(mortalityByAge);
		builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		builder.SetTestAccuracy(testAccuracy);
		//builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		ASSERT_THROW(builder.CreateDisease(), std::logic_error);
	}

	TEST_F(DiseaseBuilderTest, BuildDiseaseIsSameAsConstructed)
	{
		DiseaseSpreadSimulation::DiseaseBuilder builder;
		builder.SetDiseaseName(name);
		builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		builder.SetDaysInfectious(daysInfectious);
		builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		builder.SetMortalityByAge(mortalityByAge);
		builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		builder.SetTestAccuracy(testAccuracy);
		builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		auto builtDisease = builder.CreateDisease();

		ASSERT_TRUE(builtDisease.isSame(disease));
	}
	TEST_F(DiseaseBuilderTest, IDHasAdvanced)
	{
		DiseaseSpreadSimulation::DiseaseBuilder builder;
		builder.SetDiseaseName(name);
		builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		builder.SetDaysInfectious(daysInfectious);
		builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		builder.SetMortalityByAge(mortalityByAge);
		builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		builder.SetTestAccuracy(testAccuracy);
		builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		auto builtDisease = builder.CreateDisease();

		DiseaseSpreadSimulation::Disease disease1{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};

		ASSERT_FALSE(builtDisease == disease);
		ASSERT_TRUE(builtDisease.GetID() > disease.GetID());

		builder.SetDiseaseName(name);
		builder.SetIncubationPeriod(incubationPeriod.first, incubationPeriod.second);
		builder.SetDaysInfectious(daysInfectious);
		builder.SetDiseaseDuration(diseaseDurationRange.first, diseaseDurationRange.second);
		builder.SetMortalityByAge(mortalityByAge);
		builder.SetDaysTillDeath(daysTillDeathRange.first, daysTillDeathRange.second);
		builder.SetSpreadFactor(spreadFactor.first, spreadFactor.second);
		builder.SetTestAccuracy(testAccuracy);
		builder.SetSymptomsDevelopment(symptomsDevelopment.first, symptomsDevelopment.second);

		auto builtDisease2 = builder.CreateDisease();

		ASSERT_TRUE(builtDisease2.GetID() > builtDisease.GetID());
	}
	TEST_F(DiseaseBuilderTest, CreateCorona)
	{
		std::string coronaName = "COVID-19";
		static constexpr uint32_t coronaIncubationPeriodMin{1};
		static constexpr uint32_t coronaIncubationPeriodMax{14};
		std::pair<uint32_t, uint32_t> coronaIncubationPeriod{coronaIncubationPeriodMin, coronaIncubationPeriodMax};
		uint32_t coronaDaysInfectious = 10;
		static constexpr uint32_t coronaDurationMin{14};
		static constexpr uint32_t coronaDurationMax{56};
		std::pair<uint32_t, uint32_t> coronaDiseaseDurationRange{coronaIncubationPeriodMin + coronaDurationMin, coronaIncubationPeriodMax + coronaDurationMax};
		std::vector<float> coronaMortalityByAge{0.0f, 0.0014f, 0.0012f, 0.002f, 0.0038f, 0.0098f, .0298f, .0794f, .1734f};
		std::pair<uint32_t, uint32_t> coronaDaysTillDeathRange{coronaDurationMin, coronaDurationMax};
		std::pair<float, float> coronaSpreadFactor{0.0f, 0.5f};
		float coronaTestAccuracy = 0.981f;
		std::pair<float, float> coronaSymptomsDevelopment{0.55f, 0.85f};

		DiseaseSpreadSimulation::Disease corona{coronaName, coronaIncubationPeriod, coronaDaysInfectious, coronaDiseaseDurationRange, coronaMortalityByAge, coronaDaysTillDeathRange, coronaSpreadFactor, coronaTestAccuracy, coronaSymptomsDevelopment};

		DiseaseSpreadSimulation::DiseaseBuilder builder;
		auto builtCorona = builder.CreateCorona();

		EXPECT_TRUE(builtCorona.isSame(corona));
	}
	TEST_F(DiseaseBuilderTest, SaveCreateDeseaseFromFile)
	{
		std::string fileName{"testDiseaseFile.txt"};

		DiseaseSpreadSimulation::DiseaseBuilder builder;
		auto corona = builder.CreateCorona();

		builder.SaveDiseaseToFile("Corona1", corona, fileName);

		auto savedDiseases = builder.CreateDiseasesFromFile(fileName);

		// We check the front here because after the first run that file will contain both diseases until clean build
		EXPECT_TRUE(savedDiseases.front().isSame(corona));

		builder.SaveDiseaseToFile("Corona2", corona, fileName);

		EXPECT_TRUE(savedDiseases.back().isSame(corona));
		EXPECT_TRUE(savedDiseases.front().isSame(corona));
	}
} // namespace UnitTests
