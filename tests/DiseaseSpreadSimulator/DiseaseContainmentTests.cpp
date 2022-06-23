#include <gtest/gtest.h>
#include <cstdint>
#include <string>
#include <vector>
#include <utility>
#include "Enums.h"
#include "Places/Community.h"
#include "Places/Places.h"
#include "Person/Person.h"
#include "Person/PersonBehavior.h"
#include "Simulation/TimeManager.h"
#include "Disease/Disease.h"
#include "Disease/DiseaseContainment.h"

namespace UnitTests
{
	class DiseaseContainmentTest : public ::testing::Test
	{
	protected:
		// Person
		DiseaseSpreadSimulation::Home home{};
		DiseaseSpreadSimulation::PersonBehavior behavior{0U, 1U, 1.F, 0.F};
		DiseaseSpreadSimulation::Person person{DiseaseSpreadSimulation::Age_Group::UnderThirty, DiseaseSpreadSimulation::Sex::Male, behavior, &community, &home};

		// Community
		DiseaseSpreadSimulation::HardwareStore hwStore;
		DiseaseSpreadSimulation::Supply supplyStore;
		DiseaseSpreadSimulation::Morgue morgue;
		DiseaseSpreadSimulation::Workplace work;
		DiseaseSpreadSimulation::School school;
		DiseaseSpreadSimulation::Community community{0U, DiseaseSpreadSimulation::Country::USA};
		DiseaseSpreadSimulation::TimeManager time;
		// Values from Person.h
		static constexpr uint32_t shopOpenTime{7U};
		static constexpr uint32_t shopCloseTime{20U};
		static constexpr uint32_t workStartTime{8U};
		static constexpr uint32_t workFinishTime{17U};
		static constexpr uint32_t schoolStartTime{8U};
		static constexpr uint32_t schoolFinishTime{15U};

		void InitCommunity()
		{

			community.AddPlace(home);
			community.AddPlace(hwStore);
			community.AddPlace(supplyStore);
			community.AddPlace(morgue);
			community.AddPlace(work);
			community.AddPlace(school);
		}
	};
	// Ignore test complexity here. No point in splitting it.
	// NOLINTBEGIN(*-complexity)
	TEST_F(DiseaseContainmentTest, Quarantine)
	{
		InitCommunity();

		DiseaseSpreadSimulation::DiseaseContainment containment;

		ASSERT_EQ(person.GetWhereabouts(), person.GetHome());
		ASSERT_FALSE(person.IsQuarantined());

		for (auto i = shopOpenTime; i <= shopCloseTime; i++)
		{
			person.Update(i, true, false);
			if (person.GetWhereabouts()->GetType() == DiseaseSpreadSimulation::Place_Type::Supply)
			{
				ASSERT_EQ(person.GetWhereabouts(), community.GetSupplyStore());
				containment.Quarantine(&person);
				break;
			}
		}
		EXPECT_EQ(person.GetWhereabouts(), person.GetHome());
		EXPECT_TRUE(person.IsQuarantined());

		// Advance day and check if the person will stay at home
		constexpr uint32_t testCount{10U};
		for (uint32_t day = 0U; day < testCount; day++)
		{
			person.Update(1, true, true);

			for (auto hour = shopOpenTime; hour <= shopCloseTime; hour++)
			{
				person.Update(hour, true, false);
				EXPECT_EQ(person.GetWhereabouts(), person.GetHome());
			}
		}
	}
	// NOLINTEND(*-complexity)
	TEST_F(DiseaseContainmentTest, ReleaseWhenRecovered)
	{
		InitCommunity();
		// Disease
		std::string name{"a"};
		std::pair<uint32_t, uint32_t> incubationPeriod{2U, 2U};
		uint32_t daysInfectious{1U};
		std::pair<uint32_t, uint32_t> diseaseDurationRange{3U, 3U};
		std::vector<float> mortalityByAge{0.F, 0.F, 0.F, 0.F, 0.F, 0.F, 0.F, 0.F, 0.F};
		std::pair<uint32_t, uint32_t> daysTillDeathRange{1U, 1U};
		std::pair<float, float> spreadFactor{1.F, 1.F};
		float testAccuracy{1.0F};
		std::pair<float, float> symptomsDevelopment{1.F, 1.F};
		DiseaseSpreadSimulation::Disease disease{name, incubationPeriod, daysInfectious, diseaseDurationRange, mortalityByAge, daysTillDeathRange, spreadFactor, testAccuracy, symptomsDevelopment};

		DiseaseSpreadSimulation::DiseaseContainment containment;

		containment.Quarantine(&person);
		ASSERT_EQ(person.GetWhereabouts(), person.GetHome());
		ASSERT_TRUE(person.IsQuarantined());

		person.Contaminate(&disease);
		do
		{
			person.Update(1, false, true);
		} while (!person.HasRecovered());

		containment.ReleaseWhenRecovered(&person);
		EXPECT_FALSE(person.IsQuarantined());

		for (auto i = shopOpenTime; i <= shopCloseTime; i++)
		{
			person.Update(i, true, false);
			if (person.GetWhereabouts()->GetType() == DiseaseSpreadSimulation::Place_Type::Supply)
			{
				EXPECT_EQ(person.GetWhereabouts(), community.GetSupplyStore());
				break;
			}
		}
	}
} // namespace UnitTests
