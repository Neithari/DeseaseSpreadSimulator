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

		/// <measure>
		Measure::MeasureTime measure("\t\t\t\t\t\t\tUpdate");
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
			/// <measure>
			{
				Measure::MeasureTime measure("Person.Update");
			for (auto& person : population)
			{
				person.Update(currentTime, currentDay);
			}
			}/// </measure>

			Contacts(*community);
		}

		if (withPrint)
		{
			Print(currentDay, currentTime);
		}

		elapsedHours++;
	}
}

void DeseaseSpreadSimulation::Simulation::Print(uint64_t currentDay, uint16_t currentTime)
{
	// Only print once per hour
	//PrintEveryHour(currentTime);
	// Only print once per day
	PrintOncePerDay(currentDay, currentTime);
}

void DeseaseSpreadSimulation::Simulation::PrintEveryHour(uint16_t currentTime)
{
	for (size_t i = 0; i < communities.size(); i++)
	{
		auto& community = communities.at(i);

		fmt::print("\nCommunity #{} Day: {} Time : {} o'clock\n", i + 1, elapsedDays, currentTime);

		PrintPopulation(community->GetPopulation());

		// Print public places
		auto& places = community->GetPlaces();
		for (auto& place : places.workplaces)
		{
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID(), place.GetPersonCount());
		}
		for (auto& place : places.schools)
		{
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID(), place.GetPersonCount());
		}
		for (auto& place : places.supplyStores)
		{
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID(), place.GetPersonCount());
		}
		for (auto& place : places.hardwareStores)
		{
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID(), place.GetPersonCount());
		}
	}
}

void DeseaseSpreadSimulation::Simulation::PrintOncePerDay(uint64_t currentDay, uint16_t currentTime)
{
	if (currentDay <= elapsedDays)
	{
		return;
	}
	/// <measure>
	Measure::MeasureTime measure("Print");
	elapsedDays = currentDay;

	for (size_t i = 0; i < communities.size(); i++)
	{
		auto& community = communities.at(i);

		fmt::print("\nCommunity #{} Day: {} Time : {} o'clock\n", i + 1, elapsedDays, currentTime);
		
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

	fmt::print("Population:   {}\n ", populationCount);
	fmt::print("Susceptible:  {}\n ", susceptible);
	fmt::print("With Desease: {}\n ", withDesease);
	fmt::print("Infectious:   {}\n ", infectious);
	fmt::print("Have died:    {}\n ", deadPeople);
}

void DeseaseSpreadSimulation::Simulation::Contacts(Community& community)
{
	/// <measure>
	{
		Measure::MeasureTime measure("\t\t\t\tContacts");
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
	}/// </measure>
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
	/// <measure>
	{
	Measure::MeasureTime measure("Build communities");
	for (size_t i = 0; i < communityCount; i++)
	{
		communities.push_back(cbuilder.CreateCommunity(populationSize, country));
		
		auto& population = communities.back()->GetPopulation();

		// Assigne homes to our population at this place until we find a better solution
		//-------------------------------------------------------------------------------------------------------------
		PersonPopulator::AssigneHomesToPopulation(population, communities.back()->GetHomes(), country);
		//std::vector<Home*> homes;
		//for (auto& home : communities.back()->GetPlaces().homes)
		//{
		//	homes.push_back(&home);
		//}
		//auto homesByMemberCount = PersonPopulator::HomesByMemberCount(populationSize, country, std::move(homes));
		//
		//for (auto& person : population)
		//{
		//	person.SetHome(PersonPopulator::AssignHome(country, person.GetAgeGroup(), homesByMemberCount));
		//}
		//-------------------------------------------------------------------------------------------------------------
		InfectRandomPerson(&deseases.back(), population);
	}
	}/// </measure>

	TimeManager::Instance().Start();
	stop = false;
	fmt::print("Setup complete\n");
}

void DeseaseSpreadSimulation::Simulation::InfectRandomPerson(const Desease* desease, std::vector<Person>& population)
{
	std::random_device seed;
	std::mt19937 generator(seed());
	std::uniform_int_distribution<size_t> distribution(0, population.size() - 1);

	population.at(distribution(generator)).Contaminate(desease);
}
