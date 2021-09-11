#pragma once

namespace DeseaseSpreadSimulation
{
	// Forward declare Person
	class Person;

	class Place
	{
	public:
		virtual std::string GetTypeName() const = 0;
		const std::vector<std::unique_ptr<Person>>& GetPeople() const;
		size_t GetPersonCount() const;
		uint32_t GetID() const;
		void AddPerson(std::unique_ptr<Person> person);
		// Will return null when that person is not in that place
		std::unique_ptr<Person> TransferPerson(uint32_t id);

		virtual ~Place() = default;

	protected:
		Place(uint32_t id);

	protected:
		uint32_t placeID = 0;
		// People inside the place
		std::vector<std::unique_ptr<Person>> people;
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

	class Work : public Place
	{
	public:
		Work();

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
