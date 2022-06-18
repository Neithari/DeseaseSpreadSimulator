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
		// Open or close shops
		void ToggleShops();
		void ToggleWorkingFromHome();
		void ToggleLockdown();
		void ToggleMaskMandate();

		bool ShopsAreOpen() const;
		bool WorkingFormHome() const;
		bool IsLockdown() const;
		bool IsMaskMandate() const;

	private:
		bool areShopsOpen = true;
		bool massWorkingFromHome = false;
		bool isLockdown = false;
		bool isMaskMandate = false;
	};
} // namespace DiseaseSpreadSimulation
