#include "pch.h"
#include "Desease/DeseaseContainment.h"

DeseaseSpreadSimulation::Home* DeseaseSpreadSimulation::DeseaseContainment::Quarantine(Person* person)
{
	person->StartQuarantine();
	return person->GetHome();
}

void DeseaseSpreadSimulation::DeseaseContainment::ReleaseWhenRecovered(Person* person)
{
	if (person->HasRecovered())
	{
		person->EndQuarantine();
	}
}

void DeseaseSpreadSimulation::DeseaseContainment::ToggleShops()
{
	areShopsOpen = !areShopsOpen;
}

void DeseaseSpreadSimulation::DeseaseContainment::ToggleWorkingFromHome()
{
	massWorkingFromHome = !massWorkingFromHome;
}

void DeseaseSpreadSimulation::DeseaseContainment::ToggleLockdown()
{
	isLockdown = !isLockdown;
}

void DeseaseSpreadSimulation::DeseaseContainment::ToggleMaskMandate()
{
	isMaskMandate = !isMaskMandate;
}

bool DeseaseSpreadSimulation::DeseaseContainment::ShopsAreOpen() const
{
	return areShopsOpen;
}

bool DeseaseSpreadSimulation::DeseaseContainment::WorkingFormHome() const
{
	return massWorkingFromHome;
}

bool DeseaseSpreadSimulation::DeseaseContainment::IsLockdown() const
{
	return isLockdown;
}

bool DeseaseSpreadSimulation::DeseaseContainment::IsMaskMandate() const
{
	return isMaskMandate;
}
