#pragma once

namespace DeseaseSpreadSimulation
{
	class Community
	{
	public:
		Community(std::set<Person> population);

		void AddPlace(std::unique_ptr<Place> place);
		void AddPerson(Person person);
		void RemovePlace(uint32_t placeID);
		void RemovePerson(const Person& person);
		Person TransferPerson(const Person& person);
		const std::set<Person>& GetPopulation() const;
		const std::vector <std::unique_ptr<Place>>& GetPlaces() const;

	private:
		std::set<Person> population;
		std::vector <std::unique_ptr<Place>> places;
	};
}
