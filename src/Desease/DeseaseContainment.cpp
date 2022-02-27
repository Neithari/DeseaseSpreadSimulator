#include "pch.h"
#include "Desease/DeseaseContainment.h"

void DeseaseSpreadSimulation::DeseaseContainment::Quarantine(Person* person) const
{
	person->StartQuarantine();
	person->whereabouts = person->GetCommunity()->TransferToHome(person);
}

void DeseaseSpreadSimulation::DeseaseContainment::ReleaseWhenRecovered(Person* person) const
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
