#pragma once
#include "Disease/Disease.h"

namespace DiseaseSpreadSimulation
{
	class DiseaseBuilder
	{
	public:
		DiseaseBuilder() = default;

		// Will initialize and create the disease for you
		Disease CreateCorona();
		Disease CreateDeadlyTestDisease();

		// Disease setup functions
		void SetDiseaseName(std::string diseaseName);
		void SetIncubationPeriod(const uint16_t minDays, const uint16_t maxDays);
		void SetDaysInfectious(const uint16_t days);
		void SetDiseaseDuration(const uint16_t minDays, const uint16_t maxDays);
		// Data in % between 0-1 and for age groups of 10 years, eg. 0-9,10-19,...,80+
		void SetMortalityByAge(std::vector<float> mortality);
		void SetDaysTillDeath(const uint16_t min, const uint16_t max);
		void SetSpreadFactor(const float minFactor, const float maxFactor);
		void SetTestAccuracy(const float accuracy);
		void SetSymptomsDevelopment(const float minPercent, const float maxPercent);

		// Call only after you called all setup functions
		Disease CreateDisease();

		// In json format
		static std::vector<Disease> CreateDiseasesFromFile(std::string fileName);
		// In json format
		static void SaveDiseaseToFile(std::string diseaseSaveName, const Disease& disease, std::string fileName);

	private:
		std::string name{};
		std::pair<uint16_t, uint16_t> incubationPeriod{};
		uint16_t daysInfectious{};
		std::pair<uint16_t, uint16_t> diseaseDurationRange{};
		std::vector<float> mortalityByAge{};
		std::pair<uint16_t, uint16_t> daysTillDeathRange{};
		std::pair<float, float> spreadFactor{};
		float testAccuracy{};
		std::pair<float, float> symptomsDevelopment{};
		// For setup
		std::array<bool, 9> setupDone{};
	};
} // namespace DiseaseSpreadSimulation
