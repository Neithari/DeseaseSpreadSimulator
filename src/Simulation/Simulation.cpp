#include "pch.h"
#include "Simulation/Simulation.h"
#include "Places/CommunityBuilder.h"
#include "Desease/DeseaseBuilder.h"

DeseaseSpreadSimulation::Simulation::Simulation(uint16_t populationSize, bool withPrint)
	:
	withPrint(withPrint),
	populationSize(populationSize)
{
}
void DeseaseSpreadSimulation::Simulation::Run()
{
	SetupEverything(1);

	while (!stop)
	{
		while (pause)
		{
			// Idle when pause is called until resumed
		}

		Update();
	}
}

void DeseaseSpreadSimulation::Simulation::Stop()
{
	stop = true;
}

void DeseaseSpreadSimulation::Simulation::Pause()
{
	pause = true;
	TimeManager::Instance().Pause();
}

void DeseaseSpreadSimulation::Simulation::Resume()
{
	pause = false;
	TimeManager::Instance().Start();
}

void DeseaseSpreadSimulation::Simulation::SetSimulationSpeedMultiplier(uint16_t multiplier)
{
	TimeManager::Instance().SetSimulationTimeMultiplier(multiplier);
}

void DeseaseSpreadSimulation::Simulation::Update()
{
	TimeManager::Instance().Update();

	for (auto& community : communities)
	{
		auto& population = community.GetPopulation();
		for (auto& person : population)
		{
			person->Update();
		}
	
		Contacts(community);
	}

	if (withPrint)
	{
		Print();
	}
}

void DeseaseSpreadSimulation::Simulation::Print()
{
	for (auto& community : communities)
	{
		std::cout << "Community #" << "\n";
		uint16_t population = 0;
		uint16_t susceptible = 0;
		uint16_t infectious = 0;

		for (auto& person : community.GetPopulation())
		{
			if (person->isAlive())
			{
				population++;
			}
			if (person->isSusceptible())
			{
				susceptible++;
			}
			if (person->isInfectious())
			{
				infectious++;
			}
		}

		std::cout << "Population:  " << population << "\n";
		std::cout << "Susceptible: " << susceptible << "\n";
		std::cout << "Infectious:  " << infectious << "\n";

		for (auto& place : community.GetPlaces())
		{
			std::cout << Place::TypeToString(place->GetType()) << " #" << place->GetID() << ": " << place->GetPersonCount() << " persons\n";
		}
	}
}

void DeseaseSpreadSimulation::Simulation::Contacts(Community& community)
{
	for (auto& place : community.GetPlaces())
	{
		// Get all susceptible and infectious people
		std::vector<Person*> susceptible;
		std::vector<Person*> infectious;
		for (auto person : place->GetPeople())
		{
			if (person->isSusceptible())
			{
				susceptible.push_back(person);
			}
			else if (person->isInfectious())
			{
				infectious.push_back(person);
			}
		}
		
		// Every infectious person has a chance to infect a susceptible person
		for (auto infectiousPerson : infectious)
		{
			for (auto susceptiblePerson : susceptible)
			{
				infectiousPerson->Contact(*susceptiblePerson);
			}
		}
	}
}

void DeseaseSpreadSimulation::Simulation::SetupEverything(uint16_t communityCount)
{
	CommunityBuilder cbuilder;
	for (size_t i = 0; i < communityCount; i++)
	{
		communities.push_back(cbuilder.CreateCommunity(populationSize, country));
	}

	TimeManager::Instance().Start();
	stop = false;
}