#include "pch.h"
#include "Disease/DiseaseBuilder.h"

DiseaseSpreadSimulation::Disease DiseaseSpreadSimulation::DiseaseBuilder::CreateCorona()
{
	// https://www.rki.de/DE/Content/InfAZ/N/Neuartiges_Coronavirus/Steckbrief.html
	// https://elemental.medium.com/from-infection-to-recovery-how-long-it-lasts-199e266fd018
	SetDiseaseName("COVID-19");
	static constexpr uint16_t coronaIncubationPeriodMin{1};
	static constexpr uint16_t coronaIncubationPeriodMax{14};
	SetIncubationPeriod(coronaIncubationPeriodMin, coronaIncubationPeriodMax);
	SetDaysInfectious(10);
	static constexpr uint16_t coronaDurationMin{14};
	static constexpr uint16_t coronaDurationMax{56};
	SetDiseaseDuration(coronaIncubationPeriodMin + coronaDurationMin, coronaIncubationPeriodMax + coronaDurationMax);
	SetMortalityByAge({0.0f, 0.0014f, 0.0012f, 0.002f, 0.0038f, 0.0098f, .0298f, .0794f, .1734f});
	SetDaysTillDeath(coronaDurationMin, coronaDurationMax);

	// According to multiple sources only few individuals infect a lot of people.
	// We use a log normal distribution with most people in the low percents because of that.
	SetSpreadFactor(0.0f, 0.5f);

	SetTestAccuracy(0.981f);
	// https://www.rki.de/DE/Content/InfAZ/N/Neuartiges_Coronavirus/Steckbrief.html -> Manifestationsindex
	SetSymptomsDevelopment(0.55f, 0.85f);

	return CreateDisease();
}

DiseaseSpreadSimulation::Disease DiseaseSpreadSimulation::DiseaseBuilder::CreateDeadlyTestDisease()
{
	SetDiseaseName("DeadlyTestDisease");
	SetIncubationPeriod(1, 1);
	SetDaysInfectious(10);
	SetDiseaseDuration(10, 10);
	SetMortalityByAge({1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f});
	SetDaysTillDeath(10, 10);
	SetSpreadFactor(1.0f, 1.0f);
	SetTestAccuracy(1.0f);
	SetSymptomsDevelopment(1.f, 1.f);

	return CreateDisease();
}

void DiseaseSpreadSimulation::DiseaseBuilder::SetDiseaseName(std::string diseaseName)
{
	setupDone[0] = true;

	name = std::move(diseaseName);
}

void DiseaseSpreadSimulation::DiseaseBuilder::SetIncubationPeriod(const uint16_t minDays, const uint16_t maxDays)
{
	setupDone[1] = true;

	incubationPeriod = {minDays, maxDays};
}

void DiseaseSpreadSimulation::DiseaseBuilder::SetDaysInfectious(const uint16_t days)
{
	setupDone[2] = true;

	daysInfectious = days;
}

void DiseaseSpreadSimulation::DiseaseBuilder::SetDiseaseDuration(const uint16_t minDays, const uint16_t maxDays)
{
	setupDone[3] = true;

	diseaseDurationRange = {minDays, maxDays};
}

void DiseaseSpreadSimulation::DiseaseBuilder::SetMortalityByAge(std::vector<float> mortality)
{
	setupDone[4] = true;

	mortalityByAge = std::move(mortality);
}

void DiseaseSpreadSimulation::DiseaseBuilder::SetDaysTillDeath(const uint16_t min, const uint16_t max)
{
	setupDone[5] = true;

	daysTillDeathRange = {min, max};
}

void DiseaseSpreadSimulation::DiseaseBuilder::SetSpreadFactor(const float minFactor, const float maxFactor)
{
	setupDone[6] = true;

	spreadFactor = {minFactor, maxFactor};
}

void DiseaseSpreadSimulation::DiseaseBuilder::SetTestAccuracy(const float accuracy)
{
	setupDone[7] = true;

	testAccuracy = accuracy;
}

void DiseaseSpreadSimulation::DiseaseBuilder::SetSymptomsDevelopment(const float minPercent, const float maxPercent)
{
	setupDone[8] = true;

	symptomsDevelopment = {minPercent, maxPercent};
}

DiseaseSpreadSimulation::Disease DiseaseSpreadSimulation::DiseaseBuilder::CreateDisease()
{
	for (bool& setupStep : setupDone)
	{
		// Will throw if you didn't setup everything befor trying to create the disease
		if (!setupStep)
		{
			throw std::logic_error("Complete setup to create a disease!");
		}
	}

	return Disease{name,
		incubationPeriod,
		daysInfectious,
		diseaseDurationRange,
		mortalityByAge,
		daysTillDeathRange,
		spreadFactor,
		testAccuracy,
		symptomsDevelopment};
}

std::vector<DiseaseSpreadSimulation::Disease> DiseaseSpreadSimulation::DiseaseBuilder::CreateDiseaseFromFile(std::string fileName)
{
	using json = nlohmann::json;
	std::ifstream diseaseJsonFile{fileName};

	if (!diseaseJsonFile)
	{

		std::cerr << fileName << " could not be opened for reading!\n";
		return {};
	}

	json diseaseJson;
	diseaseJsonFile >> diseaseJson;

	std::vector<Disease> diseases;
	for (auto& disease : diseaseJson)
	{
		diseases.push_back(disease);
	}

	return diseases;
}

void DiseaseSpreadSimulation::DiseaseBuilder::SaveDiseaseToFile(std::string diseaseSaveName, const Disease& disease, std::string fileName)
{
	using json = nlohmann::json;

	// Because of the json formatting we can't just append the new desease
	json diseaseJson;

	// We need to check if the file exists
	std::ifstream fileExists{fileName};
	json existingDiseaseJson;
	if (fileExists)
	{
		// Copy all contents
		fileExists >> existingDiseaseJson;
		// Append them
		for (auto& [key, existingDisease] : existingDiseaseJson.items())
		{
			diseaseJson.emplace(key, existingDisease);
		}
	}
	fileExists.close();

	// Append the new desease
	diseaseJson.emplace(diseaseSaveName, disease);

	std::ofstream diseaseSaveFile{fileName};

	if (!diseaseSaveFile)
	{
		std::cerr << fileName << " could not be opened for writing!\n";
		return;
	}

	// Write it
	diseaseSaveFile << std::setw(4) << diseaseJson << std::endl;
}
