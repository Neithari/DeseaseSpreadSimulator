#pragma once

class Desease
{
public:
	Desease(const std::string name, const int incubationPeriod, const int daysContagious, const std::pair<int, int> deseaseDurationRange, const std::vector<float>& mortalityByAge, const std::pair<int, int> daysTillDeathRange);

	const std::string& GetDeseaseName() const;
	const int IncubationPeriod() const;
	const int DaysContagious() const;
	const float GetMortalityByAge(const unsigned int age) const;
	const int GetDeseaseDuration() const;
	const int DaysTillDeath() const;

	const bool isSame(Desease other) const;
	const bool isFatal(const unsigned int age) const;

private:
	const std::string name;
	// incubation period in days
	const int incubationPeriod;
	// patient is contagious for x days after start of symptoms
	const int daysContagious;
	// duration min, max in days
	const std::pair<int, int> durationRange;
	// mortality in percent from 0-1, age from 0-9, 10-19, 20-29,...., >80 years
	const std::vector<float> mortalityByAge;
	const std::pair<int, int> daysTillDeathRange;
};
