#include "pch.h"
#include "Simulation/Simulation.h"
#include "Desease/DeseaseBuilder.h"
#include "Person/PersonPopulator.h"
#include "Places/PlaceBuilder.h"

DeseaseSpreadSimulation::Simulation::Simulation(uint64_t populationSize, bool withPrint)
	:
	withPrint(withPrint),
	populationSize(populationSize),
	travelInfecter(Age_Group::UnderThirty, Sex::Male, PersonBehavior(100, 100, 1.f, 1.f), nullptr)
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
		Measure::MeasureTime measure("\t\t\t\t\t\t\t\t\t\t\tUpdate");
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
			auto& population = community.GetPopulation();
			/// <measure>
			{
				Measure::MeasureTime measure("Person.Update");
				UpdatePopulation(population);
			}/// </measure>
			
			/// <measure>
			{
				Measure::MeasureTime measure("\t\t\t\tContacts");
				Contacts(community.GetPlaces(), community.GetTravelLocation());
			}/// </measure>
		}

		if (withPrint)
		{
			Print();
		}
		elapsedHours++;
	}
}

void DeseaseSpreadSimulation::Simulation::UpdatePopulation(std::vector<Person>& population)
{
	std::for_each(std::execution::par_unseq, population.begin(), population.end(),
		[this](auto& person)
		{
			person.Update(time.GetTime(), time.IsWorkday(), isNewDay);
		});
}

void DeseaseSpreadSimulation::Simulation::Contacts(Places& places, Travel& travelLocation)
{
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

	// Random number of contacts for travelers
	auto travelers = travelLocation.GetPeople();
	std::for_each(std::execution::par_unseq, travelers.begin(), travelers.end(), [this](auto traveler)
		{
			auto numberOfContacts = Random::UniformIntRange(0, 5);
			for (size_t i = 0; i < numberOfContacts; i++)
			{
				std::shared_lock<std::shared_timed_mutex> lockTravelInfecter(travelInfecterMutex);
				traveler->Contact(travelInfecter);
			}
		});
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
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID() + 1, place.GetPersonCount());
		}
		for (auto& place : places.schools)
		{
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID() + 1, place.GetPersonCount());
		}
		for (auto& place : places.supplyStores)
		{
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID() + 1, place.GetPersonCount());
		}
		for (auto& place : places.hardwareStores)
		{
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID() + 1, place.GetPersonCount());
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
	size_t traveling = 0;


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
			if (person.IsTraveling())
			{
				traveling++;
			}
		}
		else
		{
			deadPeople++;
		}
	}

	fmt::print("Population:   {}\n", populationCount);
	fmt::print("Susceptible:  {}\n", susceptible);
	fmt::print("With Desease: {}\n", withDesease);
	fmt::print("Infectious:   {}\n", infectious);
	fmt::print("Traveling:    {}\n", traveling);
	fmt::print("Have died:    {}\n", deadPeople);
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
	/// <measure>
	{
	Measure::MeasureTime measure("Build communities");
	//deseases.push_back(dbuilder.CreateCorona());
	deseases.push_back(dbuilder.CreateDeadlyTestDesease());

	for (size_t i = 0; i < communityCount; i++)
	{
		PersonPopulator populationFactory(populationSize, PersonPopulator::GetCountryDistribution(country));

		auto places = placeFactory.CreatePlaces(populationSize, country);
		auto population = populationFactory.CreatePopulation(country, places.homes, places.workplaces, places.schools);

		communities.emplace_back(std::move(population), std::move(places));
		populationFactory.AddCommunityToPopulation(&communities.back(), communities.back().GetPopulation());

		InfectRandomPerson(&deseases.back(), communities.back().GetPopulation());
	}
	
	SetupTravelInfecter(&deseases.back(), &communities.back());
	}/// </measure>

	stop = false;
	fmt::print("Setup complete\n");
}

void DeseaseSpreadSimulation::Simulation::InfectRandomPerson(const Desease* desease, std::vector<Person>& population)
{
	population.at(Random::RandomVectorIndex(population)).Contaminate(desease);
}

void DeseaseSpreadSimulation::Simulation::SetupTravelInfecter(const Desease* desease, Community* communitie)
{
	travelInfecter.Contaminate(desease);
	travelInfecter.SetCommunity(communitie);
	Home home{};
	travelInfecter.SetHome(&home);
	while (!travelInfecter.IsInfectious())
	{
		travelInfecter.Update(0, true, true);
	}
}
