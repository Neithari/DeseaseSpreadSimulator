#pragma once
#include "Person/Person.h"

namespace DiseaseSpreadSimulation
{
	class DiseaseContainment
	{
	public:
		DiseaseContainment() = default;

		// Start the quarantine and send the person home
		static void Quarantine(Person* person);
		static void ReleaseWhenRecovered(Person* person);
		// Mask mandate
		void ToggleMaskMandate();
		// Home office mandate
		void ToggleWorkingFromHome();
		// Open or close shops
		void ToggleShops();
		// Full lockdown
		void ToggleLockdown();

		[[nodiscard]] bool IsMaskMandate() const;
		[[nodiscard]] bool WorkingFormHome() const;
		[[nodiscard]] bool ShopsAreClosed() const;
		[[nodiscard]] bool IsLockdown() const;

	public:
		// 50% of working people are allowed to go to work when there is a working from home mandate.
		// Reflecting jobs that are not capable of work from home
		static constexpr float percentOfJobsNoWorkFromHome{.5F};
		// During a lockdown only 10% of people are allowed to go to work
		// Reflecting jobs that are mandatory to supply people
		static constexpr float percentOfJobsMandatoryToSupply{.1F};

	private:
		bool areShopsClosed{false};
		bool massWorkingFromHome{false};
		bool isLockdown{false};
		bool isMaskMandate{false};
	};
} // namespace DiseaseSpreadSimulation
