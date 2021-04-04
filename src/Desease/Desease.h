#pragma once

namespace DeseaseSpreadSimulation
{
	enum class Seir_State { Susceptible, Exposed, Infectious, Recovered };

	// Age groups to differentiate between parts of the population from 0-9 years as first group up to > than 80 years as last group
	enum class Age_Group { UnderTen, UnderTwenty, UnderThirty, UnderFourty, UnderFifty, UnderSixty, UnderSeventy, UnderEighty, AboveEighty };

	class Desease
	{
	public:
		Desease(std::string name, const int id, const int incubationPeriod, const int daysInfectious, std::pair<int, int> deseaseDurationRange, std::vector<float> mortalityByAge, std::pair<int, int> daysTillDeathRange);

		const std::string& GetDeseaseName() const;
		int IncubationPeriod() const;
		int DaysInfectious() const;
		float GetMortalityByAge(const unsigned int age) const;
		int GetDeseaseDuration() const;
		int DaysTillDeath() const;

		unsigned int GetID() const;
		bool isSame(const Desease& other) const;
		bool hasSameID(const Desease& other) const;
		bool isFatal(const unsigned int age) const;

	private:
		const std::string name{};
		const unsigned int id = 0;
		// incubation period in days
		const int incubationPeriod = 0;
		// patient is contagious for x days after start of symptoms
		const int daysInfectious = 0;
		// duration min, max in days
		const std::pair<int, int> durationRange{};
		// mortality in percent from 0-1, age from 0-9, 10-19, 20-29,...., >80 years
		const std::vector<float> mortalityByAge;
		const std::pair<int, int> daysTillDeathRange{};
	};
}
