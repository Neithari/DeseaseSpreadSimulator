#pragma once

namespace DeseaseSpreadSimulation
{
	class DeseaseBuilder
	{
	public:
		DeseaseBuilder() = default;

		// Will initialize and create the desease for you
		Desease CreateCorona();
		Desease CreateDeadlyTestDesease();

		// Desease setup functions
		void SetDeseaseName(std::string name);
		void SetIncubationPeriod(const uint16_t period);
		void SetDaysInfectious(const uint16_t days);
		void SetDeseaseDuration(const uint16_t min, const uint16_t max);
		// Data in % between 0-1 and for age groups of 10 years, eg. 0-9,10-19,...,80+
		void SetMortalityByAge(std::vector<float> mortality);
		void SetDaysTillDeath(const uint16_t min, const uint16_t max);
		void SetSpreadFactor(const float factor);
		void SetTestAccuracy(const float accuracy);
		void SetSymptomsDevelopment(const float minPercent, const float maxPercent);

		// Call only after you called all setup functions
		Desease CreateDesease();

		Desease CreateDeseaseFromFile(std::string fileName);
		void SaveDeseaseToFile(std::string fileName) const;

	private:
		std::string name = "";
		uint16_t incubationPeriod = 0;
		uint16_t daysInfectious = 0;
		std::pair<uint16_t, uint16_t> deseaseDurationRange{ 0,0 };
		std::vector<float> mortalityByAge = {};
		std::pair<uint16_t, uint16_t> daysTillDeathRange{ 0,0 };
		float spreadFactor = 1.0f;
		float testAccuracy = 1.0f;
		std::pair<float, float> symptomsDevelopment{ 1.0f, 1.0f };
		// For setup
		bool setupDone[9] = {};
	};
}
