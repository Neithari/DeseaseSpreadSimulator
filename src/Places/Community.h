#pragma once

namespace DeseaseSpreadSimulation
{
	class Community
	{
	public:
		Community() = default;

		void AddPlace(std::unique_ptr<Place> place);
		void AddPerson(std::unique_ptr<Person> person);
		void RemovePlace(uint32_t placeID);
		void RemovePerson(uint32_t personID);
		std::unique_ptr<Person> TransferPerson(uint32_t personID);
		const std::vector<std::unique_ptr<Person>>& GetPopulation() const;
		const std::vector<std::unique_ptr<Place>>& GetPlaces() const;

	private:
		std::vector<std::unique_ptr<Person>> population;
		std::vector<std::unique_ptr<Place>> places;
	};
}
