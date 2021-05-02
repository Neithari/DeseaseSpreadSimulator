#pragma once
namespace DeseaseSpreadSimulation
{
	class Person;

	class Place
	{
	public:
		virtual std::string GetTypeName() const = 0;
		const std::vector<Person>& GetPeople() const;
		unsigned int GetID() const;

	protected:
		Place(unsigned int id);

	protected:
		unsigned int placeID = 0;
		// People inside the place
		std::vector<Person> people;
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
}
