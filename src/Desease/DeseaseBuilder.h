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
		void SetIncubationPeriod(const int period);
		void SetDaysInfectious(const int days);
		void SetDeseaseDuration(const int min, const int max);
		// Data in % between 0-1 and for age groups of 10 years, eg. 0-9,10-19,...,80+
		void SetMortalityByAge(std::vector<float> mortality);
		void SetDaysTillDeath(const int min, const int max);

		Desease CreateDeseaseFromFile(std::string fileName);
		// Call only after you called all setup functions
		Desease CreateDesease();

		void SaveDeseaseToFile(std::string fileName) const;

	private:
		std::string name = "";
		int incubationPeriod = 0;
		int daysInfectious = 0;
		std::pair<int, int> deseaseDurationRange{ 0,0 };
		std::vector<float> mortalityByAge = {};
		std::pair<int, int> daysTillDeathRange{ 0,0 };
		// For setup
		bool setupDone[6] = {};
	};
}
