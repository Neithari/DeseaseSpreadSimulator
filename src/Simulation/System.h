#pragma once
#include "Person/Person.h"
#include "Desease/DeseaseBuilder.h"

class System
{
public: // Public functions go in here
	System() = default;
	// Use only from main()
	void Run();

private: // Private functions go in here
	void Update();
	// Put everything that needs setup in that function
	void SetupEverything();

private: // Private variables go in here
	/// Test
	DeseaseSpreadSimulation::Person human{ DeseaseSpreadSimulation::Age_Group::UnderTwenty, DeseaseSpreadSimulation::Sex::Male, {30.f,30.f} };
	DeseaseSpreadSimulation::DeseaseBuilder deseases;
	/// /Test
};
