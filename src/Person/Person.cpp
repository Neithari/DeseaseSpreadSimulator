#include "pch.h"
#include "Person\Person.h"

Person::Person(unsigned int age, sf::Vector2f position)
	:
	age(age)
{
	shape.setFillColor(sf::Color::Green);
	shape.setPosition(position);
}

void Person::Update(const float deltaAdvanceTime)
{
	// move person
	Move();
	//  if we have a desease...
	if (desease)
	{
		// ...advance day
		AdvanceDay(deltaAdvanceTime);
		// ...desease check
		DeseaseCheck();
	}
}

void Person::Render(sf::RenderTarget& target) const
{
	target.draw(shape);
}

void Person::Contact(Person& other)
{
	// if the other person is contagious and I have no desease, now I have
	if (other.contagious && desease == nullptr)
	{
		Contaminate(other.desease);
		other.spreadCount++;
	}
	// if I am contagious and the other person has no desease, now he has
	if (contagious && other.desease == nullptr)
	{
		other.Contaminate(desease);
		spreadCount++;
	}
}

const std::string Person::GetDeseaseName() const
{
	if (desease)
	{
		return desease->GetDeseaseName();
	}
	return std::string();
}

void Person::Contaminate(const Desease* infection)
{
	desease = infection;
	
	daysTillOutbreak = (float)desease->IncubationPeriod();
	daysContagious = (float)desease->DaysContagious();
	daysTillCured = (float)desease->GetDeseaseDuration();
	// check if the person will die from the infection
	if (desease->isFatal(age))
	{
		willDie = true;
		daysToLive = (float)desease->DaysTillDeath();
	}

	// set shape color to represent an infection
	shape.setFillColor(sf::Color::Cyan);
}

const bool Person::isContagious() const
{
	return contagious;
}

void Person::DeseaseCheck()
{
	if (desease)
	{
		if (!contagious)
		{
			// if daysContagious is 0 person had the desease already but is not fully cured
			if (daysTillOutbreak <= 0 && daysContagious > 0)
			{
				// person is contagious when it has a desease and daysTillOutbreak reached 0
				contagious = true;

				// set shape color to represent contagious
				shape.setFillColor(sf::Color::Red);
			}
		}
		else
		{
			if (daysContagious <= 0)
			{
				contagious = false;

				// set shape color to represent an infection
				shape.setFillColor(sf::Color::Cyan);
			}
		}
		if (daysTillCured <= 0)
		{
			contagious = false;
			willDie = false;
			desease = nullptr;

			// set shape color to represent being cured
			shape.setFillColor(sf::Color::Blue);
		}
	}
}

void Person::Move()
{
	shape.move(speed);
}

void Person::AdvanceDay(const float deltaAdvanceTime)
{
	if (desease)
	{
		if (!contagious && daysTillOutbreak > 0.0f)
		{
			daysTillOutbreak -= deltaAdvanceTime;
		}
		else if (daysContagious > 0.0f)
		{
			daysContagious -= deltaAdvanceTime;
		}
		if (daysTillCured > 0.0f)
		{
			daysTillCured -= deltaAdvanceTime;
		}
		if (willDie)
		{
			daysToLive -= deltaAdvanceTime;
		}
	}
}
