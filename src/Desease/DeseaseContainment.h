#pragma once

namespace DeseaseSpreadSimulation
{
	class DeseaseContainment
	{
	public:
		DeseaseContainment() = default;

		Home* Quarantine(Person* person);
		void ReleaseWhenRecovered(Person* person);
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
}
