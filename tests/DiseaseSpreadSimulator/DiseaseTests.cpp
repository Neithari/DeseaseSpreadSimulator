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
	::testing::AssertionResult IsBetweenInclusive(T val, std::pair<T, T> range)
	{
		if ((val >= range.first) && (val <= range.second))
		{
			return ::testing::AssertionSuccess();
		}
		else
		{
			return ::testing::AssertionFailure() << val << " is outside the range " << range.first << " to " << range.second;
		}
	}
	// Don't warn on magic numbers for tests
	// NOLINTBEGIN(*-magic-numbers)
	class DiseaseTest : public ::testing::Test
	{
	protected:
		std::string name = "a";
		std::pair<uint32_t, uint32_t> incubationPeriod{1U, 1U};
		uint32_t daysInfectious = 1;
		std::pair<uint32_t, uint32_t> diseaseDurationRange{2U, 10U};
		std::vector<float> mortalityByAge{1.F, 2.F, 3.F, 4.F, 5.F, 6.F, 7.F, 8.F, 9.F};
		std::pair<uint32_t, uint32_t> daysTillDeathRange{1U, 2U};
		std::pair<float, float> spreadFactor{1.F, 1.F};
		float testAccuracy{1.F};
		std::pair<float, float> symptomsDevelopment{1.F, 1.F};
		DiseaseSpreadSimulation::Disease disease{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};

		// Helper function to check if the given value is within the range
		static bool inRange(uint32_t value, std::pair<uint32_t, uint32_t> range)
		{
			return value >= range.first && value <= range.second;
		}
	};
	// NOLINTEND(*-magic-numbers)

	TEST_F(DiseaseTest, TrivialDiseaseReturns) // cppcheck-suppress syntaxError
	{
		EXPECT_EQ(disease.GetDiseaseName(), name);
		EXPECT_TRUE(IsBetweenInclusive(disease.IncubationPeriod(), incubationPeriod));
		EXPECT_EQ(disease.DaysInfectious(), daysInfectious);
	}
	TEST_F(DiseaseTest, DiseaseDurationRange)
	{
		// Check 10 times to get an average
		static constexpr int testRuns{10};

		uint32_t duration = 0;
		for (int i = 0; i < testRuns; i++)
		{
			duration = disease.GetDiseaseDuration();
			ASSERT_TRUE(inRange(duration, diseaseDurationRange)) << "Disease duration: " << duration << " is not between: " << diseaseDurationRange.first << " and " << diseaseDurationRange.second;
		}
	}
	TEST_F(DiseaseTest, DaysTillDeathRange)
	{
		// Check 10 times to get an average
		static constexpr int testRuns{10};

		uint32_t days = 0;
		for (int i = 0; i < testRuns; i++)
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
	// There is no point in splitting this test
	// NOLINTBEGIN(*-cognitive-complexity)
	TEST_F(DiseaseTest, MortalityByAge)
	{
		// Check age 0-89
		uint32_t age{0U};
		for (uint32_t index{0U}; index < mortalityByAge.size(); index++)
		{
			constexpr auto digits{10U};
			for (uint32_t i{0U}; i < digits; i++)
			{
				// Age will be between 0 and 89
				age = index * digits + i;
				// mortalityByAge vector does only have 9 members so prevent an out of bound
				if (index < mortalityByAge.size())
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
		for (age = 90; age < 111; age++) // NOLINT(*-magic-numbers)
		{
			EXPECT_FLOAT_EQ(disease.GetMortalityByAge(age), mortalityByAge.back());
		}
	}
	// NOLINTEND(*-cognitive-complexity)
	TEST_F(DiseaseTest, EqualsOperator)
	{
		ASSERT_TRUE(disease == disease);

		DiseaseSpreadSimulation::Disease disease1{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		ASSERT_FALSE(disease == disease1);
	}
	TEST_F(DiseaseTest, IsSame)
	{
		ASSERT_TRUE(disease.isSame(disease));

		const std::string name1 = "b";
		DiseaseSpreadSimulation::Disease disease1{name1, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease1.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease1));

		const std::pair<uint32_t, uint32_t> incubationPeriod1{2, 2};
		DiseaseSpreadSimulation::Disease disease3{name, incubationPeriod1, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease3.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease3));

		const uint32_t daysInfectious1 = 2;
		DiseaseSpreadSimulation::Disease disease4{name, incubationPeriod, daysInfectious1, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease4.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease4));

		const std::pair<uint32_t, uint32_t> diseaseDurationRange1{0, 11};
		DiseaseSpreadSimulation::Disease disease5{name, incubationPeriod, daysInfectious, diseaseDurationRange1, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease5.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease5));

		const std::vector<float> mortalityByAge1{0.F, 2.F, 3.F, 4.F, 5.F, 6.F, 7.F, 8.F, 9.F};
		DiseaseSpreadSimulation::Disease disease6{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge1, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease6.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease6));

		const std::pair<uint32_t, uint32_t> daysTillDeathRange1{0, 3};
		DiseaseSpreadSimulation::Disease disease7{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange1, spreadFactor, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease7.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease7));

		const std::pair<float, float> spreadFactor1{2.F, 2.F};
		DiseaseSpreadSimulation::Disease disease8{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor1, testAccuracy, symptomsDevelopment};
		EXPECT_FALSE(disease8.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease8));

		const float testAccuracy1{2.F};
		DiseaseSpreadSimulation::Disease disease9{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy1, symptomsDevelopment};
		EXPECT_FALSE(disease9.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease9));

		const std::pair<float, float> symptomsDevelopment1{2.F, 2.F};
		DiseaseSpreadSimulation::Disease disease10{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment1};
		EXPECT_FALSE(disease10.isSame(disease));
		EXPECT_FALSE(disease.isSame(disease10));
	}

	// Don't warn on magic numbers for tests
	// NOLINTBEGIN(*-magic-numbers)
	class DiseaseBuilderTest : public ::testing::Test
	{
	protected:
		std::string name = "a";
		std::pair<uint32_t, uint32_t> incubationPeriod{1U, 1U};
		uint32_t daysInfectious = 1U;
		std::pair<uint32_t, uint32_t> diseaseDurationRange{2U, 10U};
		std::vector<float> mortalityByAge{1.F, 2.F, 3.F, 4.F, 5.F, 6.F, 7.F, 8.F, 9.F};
		std::pair<uint32_t, uint32_t> daysTillDeathRange{1U, 2U};
		std::pair<float, float> spreadFactor{1.F, 1.F};
		float testAccuracy{1.F};
		std::pair<float, float> symptomsDevelopment{1.F, 1.F};

		DiseaseSpreadSimulation::Disease disease{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};
	};
	// NOLINTEND(*-magic-numbers)
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
		static const std::string coronaName = "COVID-19";
		static constexpr uint32_t coronaIncubationPeriodMin{1};
		static constexpr uint32_t coronaIncubationPeriodMax{14};
		static constexpr std::pair<uint32_t, uint32_t> coronaIncubationPeriod{coronaIncubationPeriodMin, coronaIncubationPeriodMax};
		static constexpr uint32_t coronaDaysInfectious{10};
		static constexpr uint32_t coronaDurationMin{14};
		static constexpr uint32_t coronaDurationMax{56};
		static constexpr std::pair<uint32_t, uint32_t> coronaDiseaseDurationRange{coronaIncubationPeriodMin + coronaDurationMin, coronaIncubationPeriodMax + coronaDurationMax};
		static const std::vector<float> coronaMortalityByAge{0.0F, 0.0014F, 0.0012F, 0.002F, 0.0038F, 0.0098F, .0298F, .0794F, .1734F};
		static constexpr std::pair<uint32_t, uint32_t> coronaDaysTillDeathRange{coronaDurationMin, coronaDurationMax};
		static constexpr std::pair<float, float> coronaSpreadFactor{0.0F, 0.5F};
		static constexpr float coronaTestAccuracy{0.981F};
		static constexpr std::pair<float, float> coronaSymptomsDevelopment{0.55F, 0.85F};

		DiseaseSpreadSimulation::Disease corona{coronaName, coronaIncubationPeriod, coronaDaysInfectious, coronaDiseaseDurationRange, coronaMortalityByAge, coronaDaysTillDeathRange, coronaSpreadFactor, coronaTestAccuracy, coronaSymptomsDevelopment};

		DiseaseSpreadSimulation::DiseaseBuilder builder;
		auto builtCorona = builder.CreateCorona();

		EXPECT_TRUE(builtCorona.isSame(corona));
	}
	TEST_F(DiseaseBuilderTest, SaveCreateDeseaseFromFile)
	{
		std::string filename{"testDiseaseFile.json"};

		DiseaseSpreadSimulation::DiseaseBuilder builder;
		auto corona = builder.CreateCorona();

		builder.SaveDiseaseToFile("Corona1", corona, filename);

		auto savedDiseases = builder.CreateDiseasesFromFile(filename);

		// We check the front here because after the first run that file will contain both diseases until clean build
		EXPECT_TRUE(savedDiseases.front().isSame(corona));

		builder.SaveDiseaseToFile("Corona2", corona, filename);

		EXPECT_TRUE(savedDiseases.back().isSame(corona));
		EXPECT_TRUE(savedDiseases.front().isSame(corona));
	}
} // namespace UnitTests
