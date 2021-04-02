#pragma once

namespace DeseaseSpreadSimulation
{
	class Desease
	{
	public:
		Desease(std::string name, const int id, const int incubationPeriod, const int daysContagious, std::pair<int, int> deseaseDurationRange, std::vector<float> mortalityByAge, std::pair<int, int> daysTillDeathRange);

		const std::string& GetDeseaseName() const;
		int IncubationPeriod() const;
		int DaysContagious() const;
		float GetMortalityByAge(const unsigned int age) const;
		int GetDeseaseDuration() const;
		int DaysTillDeath() const;

		unsigned int GetID() const;
		bool isSame(const Desease& other) const;
		bool isFatal(const unsigned int age) const;

	private:
		const std::string name{};
		const unsigned int id = 0;
		// incubation period in days
		const int incubationPeriod = 0;
		// patient is contagious for x days after start of symptoms
		const int daysContagious = 0;
		// duration min, max in days
		const std::pair<int, int> durationRange{};
		// mortality in percent from 0-1, age from 0-9, 10-19, 20-29,...., >80 years
		const std::vector<float> mortalityByAge;
		const std::pair<int, int> daysTillDeathRange{};
	};
}
