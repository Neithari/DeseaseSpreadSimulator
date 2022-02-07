#include "pch.h"
#include "Simulation/Simulation.h"
#include "Desease/DeseaseBuilder.h"
#include "Person/PersonPopulator.h"
#include "Places/PlaceBuilder.h"

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
}

void DeseaseSpreadSimulation::Simulation::Resume()
{
	pause = false;
}

void DeseaseSpreadSimulation::Simulation::Update()
{
	time.Update();

	isNewDay = CheckForNewDay();

	while (elapsedHours <= time.GetElapsedHours())
	{
		for (auto& community : communities)
		{
			/// <measure>
			{
				Measure::MeasureTime measure("Person.Update");
				auto& population = community.GetPopulation();
				auto currentTime = time.GetTime();
				bool isWorkday = time.IsWorkday();
				bool tmpIsNewDay = isNewDay;

				std::for_each(std::execution::par_unseq, population.begin(), population.end(),
					[currentTime, isWorkday, tmpIsNewDay](auto& person)
					{
						person.Update(currentTime, isWorkday, tmpIsNewDay);
					});
			}/// </measure>

			Contacts(community);
		}

		if (withPrint)
		{
			Print();
		}
		elapsedHours++;
	}
}

void DeseaseSpreadSimulation::Simulation::Print()
{
	// Only print once per hour
	//PrintEveryHour();
	// Only print once per day
	if (isNewDay)
	{
		PrintOncePerDay();
	}
}

void DeseaseSpreadSimulation::Simulation::PrintEveryHour()
{
	for (size_t i = 0; i < communities.size(); i++)
	{
		auto& community = communities.at(i);

		fmt::print("\nCommunity #{} Day: {} Time : {} o'clock\n", i + 1, elapsedDays, time.GetTime());

		PrintPopulation(community.GetPopulation());

		// Print public places
		auto& places = community.GetPlaces();
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

void DeseaseSpreadSimulation::Simulation::PrintOncePerDay()
{
	/// <measure>
	Measure::MeasureTime measure("Print");

	for (size_t i = 0; i < communities.size(); i++)
	{
		auto& community = communities.at(i);

		fmt::print("\nCommunity #{} Day: {} Time : {} o'clock\n", i + 1, elapsedDays, time.GetTime());
		
		PrintPopulation(community.GetPopulation());
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
		if (person.IsAlive())
		{
			populationCount++;

			if (person.IsSusceptible())
			{
				susceptible++;
			}
			if (person.IsInfectious())
			{
				infectious++;
			}
			if (person.HasDesease())
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

	std::for_each(std::execution::par_unseq, places.homes.begin(), places.homes.end(), [](auto& place)
		{
			ContactForPlace(place);
		});
	std::for_each(std::execution::par_unseq, places.supplyStores.begin(), places.supplyStores.end(), [](auto& place)
		{
			ContactForPlace(place);
		});
	std::for_each(std::execution::par_unseq, places.workplaces.begin(), places.workplaces.end(), [](auto& place)
		{
			ContactForPlace(place);
		});
	std::for_each(std::execution::par_unseq, places.schools.begin(), places.schools.end(), [](auto& place)
		{
			ContactForPlace(place);
		});
	std::for_each(std::execution::par_unseq, places.hardwareStores.begin(), places.hardwareStores.end(), [](auto& place)
		{
			ContactForPlace(place);
		});
	}/// </measure>
}

void DeseaseSpreadSimulation::Simulation::ContactForPlace(Place& place)
{
	// Get all susceptible and infectious people
	std::vector<Person*> susceptible;
	std::vector<Person*> infectious;
	for (auto person : place.GetPeople())
	{
		if (person->IsSusceptible())
		{
			susceptible.push_back(person);
		}
		else if (person->IsInfectious())
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

bool DeseaseSpreadSimulation::Simulation::CheckForNewDay()
{
	if (time.GetElapsedDays() <= elapsedDays)
	{
		return false;
	}

	elapsedDays = time.GetElapsedDays();
	return true;
}

void DeseaseSpreadSimulation::Simulation::SetupEverything(uint16_t communityCount)
{
	DeseaseBuilder dbuilder;
	PlaceBuilder placeFactory;
	//deseases.push_back(dbuilder.CreateCorona());
	deseases.push_back(dbuilder.CreateDeadlyTestDesease());

	/// <measure>
	{
	Measure::MeasureTime measure("Build communities");
	for (size_t i = 0; i < communityCount; i++)
	{
		PersonPopulator populationFactory(populationSize, PersonPopulator::GetCountryDistribution(country));

		auto places = placeFactory.CreatePlaces(populationSize, country);
		auto population = populationFactory.CreatePopulation(populationSize, country, places.homes, places.workplaces);

		communities.emplace_back(std::move(population), std::move(places));
		populationFactory.AddCommunityToPopulation(&communities.back(), communities.back().GetPopulation());

		InfectRandomPerson(&deseases.back(), communities.back().GetPopulation());
	}
	}/// </measure>

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
