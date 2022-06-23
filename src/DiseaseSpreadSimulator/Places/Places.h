#pragma once
#include <cstdint>
#include <vector>
#include <mutex>

namespace DiseaseSpreadSimulation
{
	class Person;
	enum class Place_Type;

	class Place
	{
	public:
		[[nodiscard]] virtual Place_Type GetType() const = 0;
		std::vector<Person*>& GetPeople();
		[[nodiscard]] size_t GetPersonCount() const;
		[[nodiscard]] uint32_t GetID() const;
		// People inside the place are not owned by the place
		void AddPerson(Person* person);
		void RemovePerson(uint32_t id);
		void RemovePerson(Person* person);

		auto operator<=>(const Place& rhs) const
		{
			if (placeID < rhs.placeID)
			{
				return -1;
			}
			if (placeID > rhs.placeID)
			{
				return 1;
			}
			return 0;
		};
		inline bool operator==(const Place& rhs) const
		{
			return placeID == rhs.placeID;
		}

		static std::string TypeToString(Place_Type type);

		virtual ~Place() = default;

	protected:
		explicit Place(uint32_t id);
		Place(const Place& other);
		Place(Place&& other) noexcept;
		// Deleted here because a cast to abstract class is not allowed and we only need the derived versions
		Place& operator=(const Place& other) = delete;
		Place& operator=(Place&& other) noexcept;

	protected:
		uint32_t placeID{0};
		// People inside the place are not owned by the place
		std::vector<Person*> people{};

		std::mutex peopleMutex;
	};

	class Home : public Place
	{
	public:
		Home();
		Home(const Home& other);
		Home(Home&& other) noexcept;
		Home& operator=(const Home& other);
		Home& operator=(Home&& other) noexcept;
		~Home() override = default;

		[[nodiscard]] Place_Type GetType() const override;
	};

	class Supply : public Place
	{
	public:
		Supply();
		Supply(const Supply& other);
		Supply(Supply&& other) noexcept;
		Supply& operator=(const Supply& other);
		Supply& operator=(Supply&& other) noexcept;
		~Supply() override = default;

		[[nodiscard]] Place_Type GetType() const override;
	};

	class Workplace : public Place
	{
	public:
		Workplace();
		Workplace(const Workplace& other);
		Workplace(Workplace&& other) noexcept;
		Workplace& operator=(const Workplace& other);
		Workplace& operator=(Workplace&& other) noexcept;
		~Workplace() override = default;

		[[nodiscard]] Place_Type GetType() const override;
	};

	class School : public Place
	{
	public:
		School();
		School(const School& other);
		School(School&& other) noexcept;
		School& operator=(const School& other);
		School& operator=(School&& other) noexcept;
		~School() override = default;

		[[nodiscard]] Place_Type GetType() const override;
	};

	class HardwareStore : public Place
	{
	public:
		HardwareStore();
		HardwareStore(const HardwareStore& other);
		HardwareStore(HardwareStore&& other) noexcept;
		HardwareStore& operator=(const HardwareStore& other);
		HardwareStore& operator=(HardwareStore&& other) noexcept;
		~HardwareStore() override = default;

		[[nodiscard]] Place_Type GetType() const override;
	};
	class Morgue : public Place
	{
	public:
		Morgue();
		Morgue(const Morgue& other);
		Morgue(Morgue&& other) noexcept;
		Morgue& operator=(const Morgue& other);
		Morgue& operator=(Morgue&& other) noexcept;
		~Morgue() override = default;

		[[nodiscard]] Place_Type GetType() const override;
	private:
	};

	class Travel : public Place
	{
	public:
		Travel();
		Travel(const Travel& other);
		Travel(Travel&& other) noexcept;
		Travel& operator=(const Travel& other);
		Travel& operator=(Travel&& other) noexcept;
		~Travel() override = default;

		[[nodiscard]] Place_Type GetType() const override;
	};

	struct Places
	{
		// Insert the other places at the end of each vector
		void Insert(Places other);
	private:
		template <typename T>
		static void AppendVectorAtEnd(std::vector<T>& dest, std::vector<T>&& src)
		{
			while (!src.empty())
			{
				dest.push_back(std::move(src.back()));
				src.pop_back();
			}
		}
	public:
		std::vector<Home> homes{};
		std::vector<Supply> supplyStores{};
		std::vector<Workplace> workplaces{};
		std::vector<School> schools{};
		std::vector<HardwareStore> hardwareStores{};
		std::vector<Morgue> morgues{};
	};
} // namespace DiseaseSpreadSimulation
