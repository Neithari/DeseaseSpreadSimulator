#pragma once

namespace DeseaseSpreadSimulation
{
	// Forward declare Person
	class Person;

	class Place
	{
	public:
		virtual std::string GetTypeName() const = 0;
		const std::vector<Person*>& GetPeople() const;
		size_t GetPersonCount() const;
		uint32_t GetID() const;
		void AddPerson(Person* person);
		void RemovePerson(uint32_t id);

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
		std::string GetTypeName() const override;
	private:
	};

	class Supply : public Place
	{
	public:
		Supply();

	public:
		std::string GetTypeName() const override;
	};

	class Workplace : public Place
	{
	public:
		Workplace();

	public:
		std::string GetTypeName() const override;
	};

	class HardwareStore : public Place
	{
	public:
		HardwareStore();

	public:
		std::string GetTypeName() const override;
	};
	class Morgue : public Place
	{
	public:
		Morgue();

	public:
		std::string GetTypeName() const override;
	private:
	};
}
