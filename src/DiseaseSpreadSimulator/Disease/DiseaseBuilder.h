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
		void SetIncubationPeriod(const uint32_t minDays, const uint32_t maxDays);
		void SetDaysInfectious(const uint32_t days);
		void SetDiseaseDuration(const uint32_t minDays, const uint32_t maxDays);
		// Data in % between 0-1 and for age groups of 10 years, eg. 0-9,10-19,...,80+
		void SetMortalityByAge(std::vector<float> mortality);
		void SetDaysTillDeath(const uint32_t min, const uint32_t max);
		void SetSpreadFactor(const float minFactor, const float maxFactor);
		void SetTestAccuracy(const float accuracy);
		void SetSymptomsDevelopment(const float minPercent, const float maxPercent);

		// Call only after you called all setup functions
		Disease CreateDisease();

		// In json format
		static std::vector<Disease> CreateDiseasesFromFile(const std::string& filename);
		// In json format
		static void SaveDiseaseToFile(const std::string& diseaseSaveName, const Disease& disease, const std::string& filename);

	private:
		std::string name{};
		std::pair<uint32_t, uint32_t> incubationPeriod{};
		uint32_t daysInfectious{};
		std::pair<uint32_t, uint32_t> diseaseDurationRange{};
		std::vector<float> mortalityByAge{};
		std::pair<uint32_t, uint32_t> daysTillDeathRange{};
		std::pair<float, float> spreadFactor{};
		float testAccuracy{};
		std::pair<float, float> symptomsDevelopment{};
		// For setup
		std::array<bool, 9> setupDone{};
	};
} // namespace DiseaseSpreadSimulation
