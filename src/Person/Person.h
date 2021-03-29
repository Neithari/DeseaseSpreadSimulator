#pragma once
#include "Desease\Desease.h"
#include <SFML/Graphics.hpp>

class Person
{
public:
	Person(unsigned int age, sf::Vector2f position);

	void Update(const float deltaAdvanceTime);
	void Render(sf::RenderTarget& target) const;

	void Contact(Person& other);
	const std::string GetDeseaseName() const;
	void Contaminate(const Desease* infection);

	const bool isContagious() const;

private:
	void Move();
	// advance daysTillOutbreak, daysContagious, daysTillCured, daysToLive by a delta time
	void AdvanceDay(const float deltaAdvanceTime);
	void DeseaseCheck();

private:
	unsigned int age;
	bool contagious = false;
	unsigned int spreadCount = 0;

	const Desease* desease = nullptr;
	float daysTillOutbreak = 0.0f;
	float daysContagious = 0.0f;
	float daysTillCured = 0.0f;

	float daysToLive = 0.0f;
	bool willDie = false;

	// for render
	sf::CircleShape shape{ 10.0f };
	sf::Vector2f speed{ .02f, .01f };
	float dayOver = 1.0f;
};
