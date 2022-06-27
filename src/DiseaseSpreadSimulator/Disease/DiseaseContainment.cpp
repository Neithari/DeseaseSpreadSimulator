#include "Disease/DiseaseContainment.h"
#include "Places/Community.h"

void DiseaseSpreadSimulation::DiseaseContainment::Quarantine(Person* person)
{
	person->StartQuarantine();
	person->whereabouts = person->GetCommunity()->TransferToHome(person);
}

void DiseaseSpreadSimulation::DiseaseContainment::ReleaseWhenRecovered(Person* person)
{
	if (person->HasRecovered())
	{
		person->EndQuarantine();
	}
}

void DiseaseSpreadSimulation::DiseaseContainment::ToggleMaskMandate()
{
	isMaskMandate = !isMaskMandate;
}

void DiseaseSpreadSimulation::DiseaseContainment::ToggleWorkingFromHome()
{
	massWorkingFromHome = !massWorkingFromHome;
}

void DiseaseSpreadSimulation::DiseaseContainment::ToggleShops()
{
	areShopsOpen = !areShopsOpen;
}

void DiseaseSpreadSimulation::DiseaseContainment::ToggleLockdown()
{
	isLockdown = !isLockdown;
}

bool DiseaseSpreadSimulation::DiseaseContainment::IsMaskMandate() const
{
	return isMaskMandate;
}

bool DiseaseSpreadSimulation::DiseaseContainment::WorkingFormHome() const
{
	return massWorkingFromHome;
}

bool DiseaseSpreadSimulation::DiseaseContainment::ShopsAreClosed() const
{
	return areShopsOpen;
}

bool DiseaseSpreadSimulation::DiseaseContainment::IsLockdown() const
{
	return isLockdown;
}
