#pragma once

namespace DeseaseSpreadSimulation
{
	class Desease
	{
	public:
		Desease(std::string name, const uint16_t incubationPeriod, const uint16_t daysInfectious, std::pair<uint16_t, uint16_t> deseaseDurationRange, std::vector<float> mortalityByAge, std::pair<uint16_t, uint16_t> daysTillDeathRange, float spreadFactor = 1.0f);

		// Check with ID
		inline bool operator==(const Desease& rhs) const
		{
			return	id == rhs.id;
		};

		const std::string& GetDeseaseName() const;
		uint16_t IncubationPeriod() const;
		uint16_t DaysInfectious() const;
		float GetMortalityByAge(uint16_t age) const;
		float GetMortalityByAgeGroup(Age_Group age) const;
		uint16_t GetDeseaseDuration() const;
		uint16_t DaysTillDeath() const;
		float GetSpreadFactor() const;

		uint32_t GetID() const;
		// Check without ID
		bool isSame(const Desease& other) const;
		bool hasSameID(const Desease& other) const;
		bool isFatal(Age_Group age) const;

	private:
		const uint32_t id = 0;
		const std::string name{};
		// incubation period in days
		const uint16_t incubationPeriod = 0;
		// patient is contagious for x days after start of symptoms
		const uint16_t daysInfectious = 0;
		// duration min, max in days
		const std::pair<uint16_t, uint16_t> durationRange{};
		// mortality in percent from 0-1, age from 0-9, 10-19, 20-29,...., >80 years
		const std::vector<float> mortalityByAge;
		const std::pair<uint16_t, uint16_t> daysTillDeathRange{};
		// In percent from 0-1 how likely it is to get infected at exposure
		float spreadFactor = 1.0f;
	};
}
