#pragma once
#include "Person/Person.h"

namespace DeseaseSpreadSimulation
{
	class Simulation
	{
	public:
		Simulation(Desease desease, bool withPrint = false);

		void Start();
		void Stop();
		void Pause();

	private:
		void Update();
		void Print();

	private:
		Desease desease;
		bool withPrint = false;

		unsigned int personCount = 1000;
		std::vector<Person> persons;
	};
}
