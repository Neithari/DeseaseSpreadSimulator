#include "Disease/DiseaseContainment.h"

void DiseaseSpreadSimulation::DiseaseContainment::Quarantine(Person* person) const
{
	person->StartQuarantine();
	person->whereabouts = person->GetCommunity()->TransferToHome(person);
}

void DiseaseSpreadSimulation::DiseaseContainment::ReleaseWhenRecovered(Person* person) const
{
	if (person->HasRecovered())
	{
		person->EndQuarantine();
	}
}

void DiseaseSpreadSimulation::DiseaseContainment::ToggleShops()
{
	areShopsOpen = !areShopsOpen;
}

void DiseaseSpreadSimulation::DiseaseContainment::ToggleWorkingFromHome()
{
	massWorkingFromHome = !massWorkingFromHome;
}

void DiseaseSpreadSimulation::DiseaseContainment::ToggleLockdown()
{
	isLockdown = !isLockdown;
}

void DiseaseSpreadSimulation::DiseaseContainment::ToggleMaskMandate()
{
	isMaskMandate = !isMaskMandate;
}

bool DiseaseSpreadSimulation::DiseaseContainment::ShopsAreOpen() const
{
	return areShopsOpen;
}

bool DiseaseSpreadSimulation::DiseaseContainment::WorkingFormHome() const
{
	return massWorkingFromHome;
}

bool DiseaseSpreadSimulation::DiseaseContainment::IsLockdown() const
{
	return isLockdown;
}

bool DiseaseSpreadSimulation::DiseaseContainment::IsMaskMandate() const
{
	return isMaskMandate;
}
