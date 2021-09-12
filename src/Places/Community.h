#pragma once

namespace DeseaseSpreadSimulation
{
	class Community
	{
	public:
		Community(std::vector<Person> population);

		void AddPlace(std::unique_ptr<Place> place);
		void AddPerson(std::unique_ptr<Person> person);
		void RemovePlace(std::unique_ptr<Place> place);
		void RemovePerson(std::unique_ptr<Person> person);

	private:
		std::vector<Person> population;
		std::vector <std::unique_ptr<Place>> places;
	};
}
