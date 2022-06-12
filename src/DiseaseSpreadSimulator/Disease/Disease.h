#pragma once

namespace DiseaseSpreadSimulation
{
	class Disease
	{
	public:
		Disease(std::string name,
			const std::pair<uint16_t, uint16_t> incubationPeriod,
			const uint16_t daysInfectious,
			std::pair<uint16_t, uint16_t> diseaseDurationRange,
			std::vector<float> mortalityByAge,
			std::pair<uint16_t, uint16_t> daysTillDeathRange,
			std::pair<float, float> spreadFactor = {1.0f, 1.0f},
			float testAccuracy = 1.0f,
			std::pair<float, float> symptomsDevelopment = {1.0f, 1.0f});

		// Check with ID
		inline bool operator==(const Disease& rhs) const
		{
			return m_id == rhs.m_id;
		};

		const std::string& GetDiseaseName() const;
		uint16_t IncubationPeriod() const;
		uint16_t DaysInfectious() const;
		float GetMortalityByAge(uint16_t age) const;
		float GetMortalityByAgeGroup(Age_Group age) const;
		// Return a random duration out of the range
		uint16_t GetDiseaseDuration() const;
		// Return a random death time out of the range
		uint16_t DaysTillDeath() const;
		float GetSpreadFactor() const;
		float GetTestAccuracy() const;

		uint32_t GetID() const;
		// Check without ID
		bool isSame(const Disease& other) const;
		bool hasSameID(const Disease& other) const;
		bool isFatal(Age_Group age) const;
		bool willDevelopSymptoms() const;

		// For nlohmann/json conversion
		friend struct nlohmann::adl_serializer<Disease, void>;

	private:
		const uint32_t m_id = 0;
		const std::string m_name{};
		// Incubation period in days
		const std::pair<uint16_t, uint16_t> m_incubationPeriod{};
		// Patient is contagious for x days after start of symptoms
		const uint16_t m_daysInfectious = 0;
		// Duration min, max in days
		const std::pair<uint16_t, uint16_t> m_durationRange{};
		// Mortality in percent from 0-1, age from 0-9, 10-19, 20-29,...., >80 years
		const std::vector<float> m_mortalityByAge;
		const std::pair<uint16_t, uint16_t> m_daysTillDeathRange{};
		// In percent from 0-1 how likely it is to get infected at exposure
		std::pair<float, float> m_spreadFactor{};
		float m_testAccuracy = 1.0f;
		std::pair<float, float> m_symptomsDevelopment{};
	};
} // namespace DiseaseSpreadSimulation

// For nlohmann/json conversion
namespace nlohmann
{
	template <>
	struct adl_serializer<DiseaseSpreadSimulation::Disease>
	{
		// note: the return type is no longer 'void', and the method only takes
		// one argument
		static DiseaseSpreadSimulation::Disease from_json(const json& j)
		{
			return {
				j["Name"].get<std::string>(),
				j["Incubation Period"].get<std::pair<uint16_t, uint16_t>>(),
				j["Days Infectious"].get<uint16_t>(),
				j["Disease Duration Range"].get<std::pair<uint16_t, uint16_t>>(),
				j["Mortality By Age"].get<std::vector<float>>(),
				j["Days Till Death Range"].get<std::pair<uint16_t, uint16_t>>(),
				j["Spread Factor"].get<std::pair<float, float>>(),
				j["Test Accuracy"].get<float>(),
				j["Symptoms Development"].get<std::pair<float, float>>()};
		}

		// Here's the catch! You must provide a to_json method! Otherwise, you
		// will not be able to convert move_only_type to json, since you fully
		// specialized adl_serializer on that type
		static void to_json(json& j, const DiseaseSpreadSimulation::Disease& disease)
		{
			j = {
				{"Name", disease.m_name},
				{"Incubation Period", disease.m_incubationPeriod},
				{"Days Infectious", disease.m_daysInfectious},
				{"Disease Duration Range", disease.m_durationRange},
				{"Mortality By Age", disease.m_mortalityByAge},
				{"Days Till Death Range", disease.m_daysTillDeathRange},
				{"Spread Factor", disease.m_spreadFactor},
				{"Test Accuracy", disease.m_testAccuracy},
				{"Symptoms Development", disease.m_symptomsDevelopment}};
		}
	};
} // namespace nlohmann
