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
	auto currentHour = TimeManager::Instance().GetElapsedHours();
	auto currentDay = TimeManager::Instance().GetElapsedDays();
	auto currentTime = TimeManager::Instance().GetTime();

	while (elapsedHours <= currentHour)
	{
		for (auto& community : communities)
		{
			auto& population = community->GetPopulation();
			for (auto& person : population)
			{
				person.Update(currentTime, currentDay);
			}

			Contacts(*community);
		}

		if (withPrint)
		{
			Print(currentHour, currentDay);
		}

		elapsedHours++;
	}
}

void DeseaseSpreadSimulation::Simulation::Print(uint64_t /*currentHour*/, uint64_t currentDay)
{
	// Only print once per hour
	//PrintEveryHour(currentHour);
	// Only print once per day
	PrintOncePerDay(currentDay);
}

void DeseaseSpreadSimulation::Simulation::PrintEveryHour(uint64_t currentHour)
{
	if (currentHour <= elapsedHours)
	{
		return;
	}
	// Don't update elapsed hours here because we update it in Contacts()

	for (size_t i = 0; i < communities.size(); i++)
	{
		auto& community = communities.at(i);

		std::cout << "\nCommunity #" << i + 1 << " Day: " << TimeManager::Instance().GetElapsedDays() << " Time : " << TimeManager::Instance().GetTime() << " o'clock\n";

		PrintPopulation(community->GetPopulation());

		// Print public places
		auto& places = community->GetPlaces();
		for (auto& place : places.workplaces)
		{
			std::cout << Place::TypeToString(place.GetType()) << " #" << place.GetID() << ": " << place.GetPersonCount() << " persons\n";
		}
		for (auto& place : places.schools)
		{
			std::cout << Place::TypeToString(place.GetType()) << " #" << place.GetID() << ": " << place.GetPersonCount() << " persons\n";
		}
		for (auto& place : places.supplyStores)
		{
			std::cout << Place::TypeToString(place.GetType()) << " #" << place.GetID() << ": " << place.GetPersonCount() << " persons\n";
		}
		for (auto& place : places.hardwareStores)
		{
			std::cout << Place::TypeToString(place.GetType()) << " #" << place.GetID() << ": " << place.GetPersonCount() << " persons\n";
		}
	}
}

void DeseaseSpreadSimulation::Simulation::PrintOncePerDay(uint64_t currentDay)
{
	if (currentDay <= elapsedDays)
	{
		return;
	}
	elapsedDays = currentDay;

	for (size_t i = 0; i < communities.size(); i++)
	{
		auto& community = communities.at(i);

		std::cout << "\nCommunity #" << i + 1 << " Day: " << TimeManager::Instance().GetElapsedDays() << " Time : " << TimeManager::Instance().GetTime() << " o'clock\n";
		
		PrintPopulation(community->GetPopulation());
	}
}

void DeseaseSpreadSimulation::Simulation::PrintPopulation(const std::vector<Person>& population) const
{
	size_t populationCount = 0;
	size_t susceptible = 0;
	size_t withDesease = 0;
	size_t infectious = 0;
	size_t deadPeople = 0;

	for (auto& person : population)
	{
		if (person.isAlive())
		{
			populationCount++;

			if (person.isSusceptible())
			{
				susceptible++;
			}
			if (person.isInfectious())
			{
				infectious++;
			}
			if (person.hasDesease(deseases.back().GetDeseaseName()))
			{
				withDesease++;
			}
		}
		else
		{
			deadPeople++;
		}
	}

	std::cout << "Population:   " << populationCount << "\n";
	std::cout << "Susceptible:  " << susceptible << "\n";
	std::cout << "With Desease: " << withDesease << "\n";
	std::cout << "Infectious:   " << infectious << "\n";
	std::cout << "Have died:    " << deadPeople << "\n";
}

void DeseaseSpreadSimulation::Simulation::Contacts(Community& community)
{
	auto& places = community.GetPlaces();
	for (auto& place : places.homes)
	{
		ContactForPlace(place);
	}
	for (auto& place : places.supplyStores)
	{
		ContactForPlace(place);
	}
	for (auto& place : places.workplaces)
	{
		ContactForPlace(place);
	}
	for (auto& place : places.schools)
	{
		ContactForPlace(place);
	}
	for (auto& place : places.hardwareStores)
	{
		ContactForPlace(place);
	}
}

void DeseaseSpreadSimulation::Simulation::ContactForPlace(Place& place)
{
	// Get all susceptible and infectious people
	std::vector<Person*> susceptible;
	std::vector<Person*> infectious;
	for (auto person : place.GetPeople())
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

void DeseaseSpreadSimulation::Simulation::SetupEverything(uint16_t communityCount)
{
	DeseaseBuilder dbuilder;
	//deseases.push_back(dbuilder.CreateCorona());
	deseases.push_back(dbuilder.CreateDeadlyTestDesease());

	CommunityBuilder cbuilder;
	for (size_t i = 0; i < communityCount; i++)
	{
		communities.push_back(cbuilder.CreateCommunity(populationSize, country));
		
		auto& population = communities.back()->GetPopulation();

		// Assigne homes to our population at this place until we find a better solution
		//-------------------------------------------------------------------------------------------------------------
		std::vector<Home*> homes;
		for (auto& home : communities.back()->GetPlaces().homes)
		{
			homes.push_back(&home);
		}
		auto homesByMemberCount = PersonPopulator::HomesByMemberCount(populationSize, country, std::move(homes));

		for (auto& person : population)
		{
			person.SetHome(PersonPopulator::AssignHome(country, person.GetAgeGroup(), homesByMemberCount));
		}
		//-------------------------------------------------------------------------------------------------------------
		InfectRandomPerson(&deseases.back(), population);
	}

	std::cout << "Setup complete\n";
	TimeManager::Instance().Start();
	stop = false;
}

void DeseaseSpreadSimulation::Simulation::InfectRandomPerson(const Desease* desease, std::vector<Person>& population)
{
	std::random_device seed;
	std::mt19937 generator(seed());
	std::uniform_int_distribution<size_t> distribution(0, population.size() - 1);

	population.at(distribution(generator)).Contaminate(desease);
}
