#pragma once

namespace DeseaseSpreadSimulation
{
	class Person
	{
	public:
		Person(unsigned int age, std::pair<float, float> position);

		void Update();

		void Contact(Person& other);
		std::string GetDeseaseName() const;
		void Contaminate(const Desease* infection);
		// advance daysTillOutbreak, daysContagious, daysTillCured, daysToLive by a delta time
		void AdvanceDay();

		bool isSusceptible() const;
		bool isInfectious() const;
		bool isQuarantined() const;
		bool isAlive() const;
		bool hasDesease(const std::string& deseaseName) const;

	private:
		void Move();
		void DeseaseCheck();

	private:
		unsigned int age;
		std::pair<float, float> position;

		Seir_State state = Seir_State::Susceptible;
		bool quarantined = false;
		unsigned int spreadCount = 0;

		// Desease Stuff
		const Desease* desease = nullptr;
		unsigned int daysTillOutbreak = 0;
		unsigned int daysInfectious = 0;
		unsigned int daysTillCured = 0;

		bool alive = true;
		unsigned int daysToLive = 0;
		bool willDie = false;
	};
}
