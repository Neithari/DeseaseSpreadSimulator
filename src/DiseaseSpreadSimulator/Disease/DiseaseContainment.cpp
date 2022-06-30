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
	areShopsClosed = !areShopsClosed;
}

void DiseaseSpreadSimulation::DiseaseContainment::ToggleLockdown()
{
	isLockdown = !isLockdown;
}

void DiseaseSpreadSimulation::DiseaseContainment::SetMaskMandate(bool set)
{
	isMaskMandate = set;
}

void DiseaseSpreadSimulation::DiseaseContainment::SetWorkingFromHome(bool set)
{
	massWorkingFromHome = set;
}

void DiseaseSpreadSimulation::DiseaseContainment::SetShopsClosed(bool set)
{
	areShopsClosed = set;
}

void DiseaseSpreadSimulation::DiseaseContainment::SetLockdown(bool set)
{
	isLockdown = set;
}

void DiseaseSpreadSimulation::DiseaseContainment::ResetMaskMandate(bool set)
{
	SetMaskMandate(set);
}

void DiseaseSpreadSimulation::DiseaseContainment::ResetWorkingFromHome(bool set)
{
	SetWorkingFromHome(set);
}

void DiseaseSpreadSimulation::DiseaseContainment::ResetShopsClosed(bool set)
{
	SetShopsClosed(set);
}

void DiseaseSpreadSimulation::DiseaseContainment::ResetLockdown(bool set)
{
	SetLockdown(set);
}

bool DiseaseSpreadSimulation::DiseaseContainment::IsMaskMandate() const
{
	return isMaskMandate;
}

bool DiseaseSpreadSimulation::DiseaseContainment::WorkingFromHome() const
{
	return massWorkingFromHome;
}

bool DiseaseSpreadSimulation::DiseaseContainment::ShopsAreClosed() const
{
	return areShopsClosed;
}

bool DiseaseSpreadSimulation::DiseaseContainment::IsLockdown() const
{
	return isLockdown;
}
