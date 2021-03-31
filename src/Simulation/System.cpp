#include "pch.h"
#include "Simulation\System.h"

void System::Run()
{
	// Put everything that needs setup in that function
	SetupEverything();

	// Main loop
	while (true)
	{
		Update();
	}
}

void System::Update()
{
	human.Update();
}

void System::SetupEverything()
{
	human.Contaminate(&deseases.CreateCorona());
}
