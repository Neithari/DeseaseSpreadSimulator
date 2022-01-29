#include "pch.h"
#include "Simulation/Simulation.h"
#include "Places/CommunityBuilder.h"
#include "Desease/DeseaseBuilder.h"

DeseaseSpreadSimulation::Simulation::Simulation(uint64_t populationSize, bool withPrint)
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
	// Only print once per hour
	//PrintEveryHour();
	// Only print once per day
	PrintOncePerDay();
}

void DeseaseSpreadSimulation::Simulation::PrintEveryHour()
{
	if (TimeManager::Instance().GetElapsedHours() <= elapsedTime)
	{
		return;
	}
	elapsedTime = TimeManager::Instance().GetElapsedHours();

	for (size_t i = 0; i < communities.size(); i++)
	{
		auto& community = communities.at(i);

		std::cout << "\nCommunity #" << i + 1 << " Day: " << TimeManager::Instance().GetElapsedDays() << " Time : " << TimeManager::Instance().GetTime() << " o'clock\n";

		PrintPopulation(community.GetPopulation());

		// Print public places
		size_t deadPeople = 0;
		for (auto& place : community.GetPlaces())
		{
			if (place->GetType() == Place_Type::Home)
			{
				continue;
			}
			if (place->GetType() == Place_Type::Morgue)
			{
				deadPeople += place->GetPersonCount();
			}
			std::cout << Place::TypeToString(place->GetType()) << " #" << place->GetID() << ": " << place->GetPersonCount() << " persons\n";
		}
		std::cout << "Have died:    " << deadPeople << "\n";
	}
}

void DeseaseSpreadSimulation::Simulation::PrintOncePerDay()
{
	if (TimeManager::Instance().GetElapsedDays() <= elapsedTime)
	{
		return;
	}
	elapsedTime = TimeManager::Instance().GetElapsedDays();

	for (size_t i = 0; i < communities.size(); i++)
	{
		auto& community = communities.at(i);

		std::cout << "\nCommunity #" << i + 1 << " Day: " << TimeManager::Instance().GetElapsedDays() << " Time : " << TimeManager::Instance().GetTime() << " o'clock\n";
		
		PrintPopulation(community.GetPopulation());

		// Check morgues for dead people
		size_t deadPeople = 0;
		for (auto& place : community.GetPlaces())
		{
			if (place->GetType() == Place_Type::Morgue)
			{
				deadPeople += place->GetPersonCount();
			}
		}
		std::cout << "Have died:    " << deadPeople  << "\n";
	}
}

void DeseaseSpreadSimulation::Simulation::PrintPopulation(const std::vector<std::unique_ptr<Person>>& population) const
{
	size_t populationCount = 0;
	size_t susceptible = 0;
	size_t withDesease = 0;
	size_t infectious = 0;

	for (auto& person : population)
	{
		if (person->isAlive())
		{
			populationCount++;
		}
		if (person->isSusceptible())
		{
			susceptible++;
		}
		if (person->isInfectious())
		{
			infectious++;
		}
		if (person->hasDesease(deseases.back().GetDeseaseName()))
		{
			withDesease++;
		}
	}

	std::cout << "Population:   " << populationCount << "\n";
	std::cout << "Susceptible:  " << susceptible << "\n";
	std::cout << "With Desease: " << withDesease << "\n";
	std::cout << "Infectious:   " << infectious << "\n";
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
	DeseaseBuilder dbuilder;
	//deseases.push_back(dbuilder.CreateCorona());
	deseases.push_back(dbuilder.CreateDeadlyTestDesease());

	CommunityBuilder cbuilder;
	for (size_t i = 0; i < communityCount; i++)
	{
		communities.push_back(cbuilder.CreateCommunity(populationSize, country));
		
		auto& population = communities.back().GetPopulation();
		// Set the community until we have a better solution
		for (auto& person : population)
		{
			person->SetCommunity(&communities.back());
		}

		InfectRandomPerson(&deseases.back(), population);
	}

	TimeManager::Instance().Start();
	stop = false;
}

void DeseaseSpreadSimulation::Simulation::InfectRandomPerson(const Desease* desease, std::vector<std::unique_ptr<Person>>& population)
{
	std::random_device seed;
	std::mt19937 generator(seed());
	std::uniform_int_distribution<size_t> distribution(0, population.size() - 1);

	population.at(distribution(generator))->Contaminate(desease);
}
