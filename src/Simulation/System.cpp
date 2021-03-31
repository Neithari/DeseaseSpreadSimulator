#include "pch.h"
#include "Simulation\System.h"

// Used to ensure only one system is present at any time and the instance is globally accessible.
System System::instance;

System& System::Instance()
{
	return instance;
}

System::System(const int windowWidth, const int windowHeight)
	: windowWidth(windowWidth), windowHeight(windowHeight),
	window(sf::VideoMode(windowWidth, windowHeight), "Corona Spread Simulation")
{
	// Set the backgroundcolor

	LoadFonts();
}

void System::Run()
{
	// Used to ensure Run() is only run once
	if (runNumber > 0)
	{
		return;
	}
	runNumber++;

	// Put everything that needs setup in that function
	SetupEverything();

	// Main loop
	while (window.isOpen())
	{
		HandleSFEvents();

		Update();

		Render();
	}
}

sf::Vector2f System::GetMousePosition() const
{
	auto mousePosition = sf::Mouse::getPosition(window);
	ClampPositionToWindow(mousePosition);
	return sf::Vector2f((float)mousePosition.x, (float)mousePosition.y);
}

void System::HandleSFEvents()
{
	// Process SFML Events
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

void System::Update()
{
	human.Update();
}

void System::Render()
{
	// Clear window
	window.clear(backgroundColor);

	// render everything
	/// Test shape


	// Display rendered items
	window.display();
}

void System::SetupEverything()
{
	human.Contaminate(&deseases.CreateCorona());
}

void System::LoadFonts()
{
	if (!FontPressStart->loadFromFile("assets/fonts/press-start/prstart.ttf"))
	{
		std::cerr << "Font \"prstart.ttf\" could not be loaded" << std::endl;
	}
	if (!FontDustismoRoman->loadFromFile("assets/fonts/dustismo-roman/Dustismo_Roman.ttf"))
	{
		std::cerr << "Font \"Dustismo_Roman.ttf\" could not be loaded" << std::endl;
	}
}
