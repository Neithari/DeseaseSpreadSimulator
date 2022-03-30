#pragma once
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
	DeseaseSpreadSimulation::DeseaseBuilder deseases;
	/// /Test
};
