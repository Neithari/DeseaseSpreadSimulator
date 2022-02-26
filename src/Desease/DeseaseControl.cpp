#include "pch.h"
#include "DeseaseControl.h"

DeseaseSpreadSimulation::Home* DeseaseSpreadSimulation::DeseaseControl::Quarantine(Person* person)
{
	return person->GetHome();
}

void DeseaseSpreadSimulation::DeseaseControl::ToggleShops()
{
	areShopsOpen = !areShopsOpen;
}

void DeseaseSpreadSimulation::DeseaseControl::ToggleWorkingFromHome()
{
	massWorkingFromHome = !massWorkingFromHome;
}

void DeseaseSpreadSimulation::DeseaseControl::ToggleLockdown()
{
	isLockdown = !isLockdown;
}

void DeseaseSpreadSimulation::DeseaseControl::ToggleMaskMandate()
{
	isMaskMandate = !isMaskMandate;
}

bool DeseaseSpreadSimulation::DeseaseControl::ShopsAreOpen() const
{
	return areShopsOpen;
}

bool DeseaseSpreadSimulation::DeseaseControl::WorkingFormHome() const
{
	return massWorkingFromHome;
}

bool DeseaseSpreadSimulation::DeseaseControl::IsLockdown() const
{
	return isLockdown;
}

bool DeseaseSpreadSimulation::DeseaseControl::IsMaskMandate() const
{
	return isMaskMandate;
}
