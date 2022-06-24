#pragma once
#include <cstdint>
#include <string>
#include "Enums.h"
#include "Disease/Infection.h"
#include "Places/Places.h"
#include "Person/PersonBehavior.h"

namespace DiseaseSpreadSimulation
{
	class Disease;
	class Community;
	class DiseaseContainment;

	class Person
	{
	public:
		Person(Age_Group age, Sex sex, PersonBehavior behavior, Community* community, Home* home = nullptr);

		friend class DiseaseContainment;

		auto operator<=>(const Person& rhs) const
		{
			if (id < rhs.id)
			{
				return -1;
			}
			if (id > rhs.id)
			{
				return 1;
			}
			return 0;
		};
		inline bool operator==(const Person& rhs) const
		{
			return id == rhs.id;
		}

		void Update(uint32_t currentTime, bool isWorkday, bool isNewDay);

		// Will try to infect a susceptible person when the other is infectious
		void Contact(Person& other);
		void Contaminate(const Disease* disease);
		void Kill();

		[[nodiscard]] bool IsSusceptible() const;
		[[nodiscard]] bool IsInfectious() const;
		[[nodiscard]] bool IsQuarantined() const;
		[[nodiscard]] bool IsTraveling() const;
		[[nodiscard]] bool IsAlive() const;
		[[nodiscard]] bool HasDisease() const;
		const std::string& GetDiseaseName() const;
		[[nodiscard]] bool HasRecovered() const;

		[[nodiscard]] uint32_t GetID() const;
		[[nodiscard]] Age_Group GetAgeGroup() const;
		[[nodiscard]] Sex GetSex() const;
		[[nodiscard]] const PersonBehavior& GetBehavior() const;
		[[nodiscard]] uint32_t GetSpreadCount() const;
		[[nodiscard]] const Disease* GetDisease() const;

		Community* GetCommunity();
		Place* GetWhereabouts();
		Home* GetHome();
		Workplace* GetWorkplace();
		School* GetSchool();

		void SetHome(Home* newHome);
		void SetWorkplace(Workplace* newWorkplace);
		void SetSchool(School* newSchool);
		void SetCommunity(Community* newCommunity);

		void ChangeBehavior(PersonBehavior newBehavior);

	private:
		void CheckNextMove(uint32_t currentTime, bool& isWorkday, bool isNewDay);
		void PrepareShopping();
		void GoSupplyShopping(uint32_t currentTime);
		void GoHardwareShopping(uint32_t currentTime);
		[[nodiscard]] bool WillTravel() const;
		void StartTraveling();

		void StartQuarantine();
		void EndQuarantine();

	
		uint32_t id;
		Age_Group m_age;
		Sex m_sex;
		PersonBehavior m_behavior;
		bool alive{true};
		bool isTraveling{false};
		bool isQuarantined{false};

		// Not const because we will add ourself to the places
		Community* m_community;
		Home* m_home;
		Place* whereabouts;
		Workplace* workplace{nullptr};
		School* school{nullptr};

		Infection infection;

		// In days
		uint32_t lastFoodBuy{0U};
		uint32_t lastHardwareBuy{0U};
		uint32_t travelDays{0U};
		// In hours
		uint32_t buyTime{0U};
		uint32_t buyFinishTime{0U};
		bool isShoppingDay{false};
		// Time in x/24h
		static constexpr uint32_t shopOpenTime{7U};
		static constexpr uint32_t shopCloseTime{20U};
		static constexpr uint32_t workStartTime{8U};
		static constexpr uint32_t workFinishTime{17U};
		static constexpr uint32_t schoolStartTime{8U};
		static constexpr uint32_t schoolFinishTime{15U};
		// Percent
		static constexpr float baseTravelReturnChance{0.3F};
	};
} // namespace DiseaseSpreadSimulation
