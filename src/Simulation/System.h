#pragma once
#include "Person\Person.h"
#include "Desease\DeseaseBuilder.h"

class System
{
private: // Constructor is private to permit more than one construction
	System(const int windowWidth = 1024, const int windowHeight = 768);
public: // Constructors, destructors, assignments in here
	System(const System&) = delete;
	System& operator=(const System&) = delete;
	System(System&&) = delete;
	System& operator=(System&&) = delete;

public: // Public functions go in here
	// Access the only System instance
	static System& Instance();
	// Use only from main()
	void Run();

	// Gets the adjusted mouse position for our window
	sf::Vector2f GetMousePosition() const;

private: // Private functions go in here
	void HandleSFEvents();
	void Update();
	void Render();
	// Clamp a position to the window. THIS WILL CHANGE THE GIVEN POSITION
	template <typename T>
	void ClampPositionToWindow(T& position) const;
	// Put everything that needs setup in that function
	void SetupEverything();
	void LoadFonts();

public: // Public variables go in here
	const int windowWidth;
	const int windowHeight;

private: // Private variables go in here
	const sf::Color backgroundColor{ 51, 51, 51 };

	/// Test
	Person human{ 10,{30.f,30.f} };
	DeseaseBuilder deseases;
	/// /Test

	// Used to ensure only one system is present at any time
	static System instance;
	// Used to ensure Run() is only run once
	int runNumber;
	// The main window
	sf::RenderWindow window;
	// Fonts
	std::shared_ptr<sf::Font> FontPressStart = std::make_shared<sf::Font>();
	std::shared_ptr<sf::Font> FontDustismoRoman = std::make_shared<sf::Font>();
};

// Clamp a position to the window. THIS WILL CHANGE THE GIVEN POSITION
template<typename T>
inline void System::ClampPositionToWindow(T& position) const
{
	if (position.x < 0)
	{
		position.x = 0;
	}
	if (position.x > windowWidth)
	{
		position.x = windowWidth;
	}
	if (position.y < 0)
	{
		position.y = 0;
	}
	if (position.y > windowHeight)
	{
		position.y = windowHeight;
	}
}
