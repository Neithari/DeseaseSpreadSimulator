#pragma once

namespace DeseaseSpreadSimulation
{
	class DeseaseBuilder
	{
	public:
		DeseaseBuilder() = default;

		// will initialize and create the desease for you
		const Desease& CreateCorona();

		// desease setup functions
		void SetDeseaseName(std::string name);
		void SetIncubationPeriod(const int period);
		void SetDaysContagious(const int days);
		void SetDeseaseDuration(const int min, const int max);
		// data in % between 0-1 and for age groups of 10 years, eg. 0-9,10-19,...,80+
		void SetMortalityByAge(std::vector<float> mortality);
		void SetDaysTillDeath(const int min, const int max);

		const Desease& CreateDeseaseFromFile(std::string fileName);
		// call only after you called all setup functions
		const Desease& CreateDesease();

		void SaveDeseaseToFile(std::string fileName) const;

	private:
		static unsigned int GetID();

	private:
		std::string name = "";
		int incubationPeriod = 0;
		int daysContagious = 0;
		std::pair<int, int> deseaseDurationRange{ 0,0 };
		std::vector<float> mortalityByAge = {};
		std::pair<int, int> daysTillDeathRange{ 0,0 };
		// for setup
		bool setupDone[6] = {};

		std::vector<Desease> deseases;
	};
}
