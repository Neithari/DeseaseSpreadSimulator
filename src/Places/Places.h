#pragma once

namespace DeseaseSpreadSimulation
{
	// Forward declare Person
	class Person;

	class Place
	{
	public:
		virtual Place_Type GetType() const = 0;
		std::vector<Person*>& GetPeople();
		size_t GetPersonCount() const;
		uint32_t GetID() const;
		void AddPerson(Person* person);
		void RemovePerson(uint32_t id);
		void RemovePerson(Person* person);

		static std::string TypeToString(Place_Type type);

		virtual ~Place() = default;

	protected:
		Place(uint32_t id);

	protected:
		const uint32_t placeID = 0;
		// People inside the place not owned by place
		std::vector<Person*> people;
	};

	class Home : public Place
	{
	public:
		Home();

	public:
		Place_Type GetType() const override;
	};

	class Supply : public Place
	{
	public:
		Supply();

	public:
		Place_Type GetType() const override;
	};

	class Workplace : public Place
	{
	public:
		Workplace();

	public:
		Place_Type GetType() const override;
	};

	class School : public Place
	{
	public:
		School();

	public:
		Place_Type GetType() const override;
	};

	class HardwareStore : public Place
	{
	public:
		HardwareStore();

	public:
		Place_Type GetType() const override;
	};
	class Morgue : public Place
	{
	public:
		Morgue();

	public:
		Place_Type GetType() const override;
	private:
	};

	struct Places
	{
		// Insert the other places at the end of each vector
		void Insert(Places other);

		std::vector<Home> homes;
		std::vector<Supply> supplyStores;
		std::vector<Workplace> workplaces;
		std::vector<School> schools;
		std::vector<HardwareStore> hardwareStores;
		std::vector<Morgue> morgues;
	};
}
