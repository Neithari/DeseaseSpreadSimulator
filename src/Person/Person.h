#pragma once
#include "Desease\Desease.h"

class Person
{
public:
	Person(unsigned int age, std::pair<float, float> position);

	void Update();

	void Contact(Person& other);
	const std::string GetDeseaseName() const;
	void Contaminate(const Desease* infection);
	// advance daysTillOutbreak, daysContagious, daysTillCured, daysToLive by a delta time
	void AdvanceDay();

	const bool isReceptible() const;
	const bool isContagious() const;
	const bool isQuarantined() const;
	const bool isAlive() const;
	const bool hasDesease(const std::string& deseaseName) const;

private:
	void Move();
	void DeseaseCheck();

private:
	unsigned int age;
	std::pair<float, float> position;

	bool receptible = false;
	bool contagious = false;
	bool quarantined = false;
	unsigned int spreadCount = 0;

	// Desease Stuff
	const Desease* desease = nullptr;
	unsigned int daysTillOutbreak = 0;
	unsigned int daysContagious = 0;
	unsigned int daysTillCured = 0;

	bool alive = true;
	unsigned int daysToLive = 0;
	bool willDie = false;
};
