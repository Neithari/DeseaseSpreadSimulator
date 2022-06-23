#include "Simulation/Simulation.h"
#include <algorithm>
#include <execution>
#include "fmt/core.h"
#include "Disease/DiseaseBuilder.h"
#include "RandomNumbers.h"

DiseaseSpreadSimulation::Simulation::Simulation(uint64_t populationSize, bool withPrint)
	: m_withPrint(withPrint),
	  m_populationSize(populationSize),
	  travelInfecter(Age_Group::UnderThirty, Sex::Male, PersonBehavior(100U, 100U, 1.F, 1.F), nullptr) // NOLINT: There is no benefit in named constants here
{
}
void DiseaseSpreadSimulation::Simulation::Run()
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

void DiseaseSpreadSimulation::Simulation::Stop()
{
	stop = true;
}

void DiseaseSpreadSimulation::Simulation::Pause()
{
	pause = true;
}

void DiseaseSpreadSimulation::Simulation::Resume()
{
	pause = false;
}

void DiseaseSpreadSimulation::Simulation::Update()
{
	time.Update();

	isNewDay = CheckForNewDay();

	while (elapsedHours <= time.GetElapsedHours())
	{
		for (auto& community : communities)
		{
			auto& population = community.GetPopulation();

			UpdatePopulation(population);

			Contacts(community.GetPlaces(), community.GetTravelLocation());
		}

		if (m_withPrint)
		{
			Print();
		}
		elapsedHours++;
	}
}

void DiseaseSpreadSimulation::Simulation::UpdatePopulation(std::vector<Person>& population)
{
	std::for_each(std::execution::par_unseq, population.begin(), population.end(), [this](auto& person)
		{
			person.Update(time.GetTime(), time.IsWorkday(), isNewDay);
		});
}

void DiseaseSpreadSimulation::Simulation::Contacts(Places& places, Travel& travelLocation)
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
			auto numberOfContacts = Random::UniformIntRange(minTravelContacts, maxTravelContacts);
			for (auto i = 0U; i < numberOfContacts; i++)
			{
				std::shared_lock<std::shared_timed_mutex> lockTravelInfecter(travelInfecterMutex);
				traveler->Contact(travelInfecter);
			}
		});
}

void DiseaseSpreadSimulation::Simulation::ContactForPlace(Place& place)
{
	// Get all susceptible and infectious people
	std::vector<Person*> susceptible{};
	std::vector<Person*> infectious{};
	for (auto* person : place.GetPeople())
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
	for (auto* infectiousPerson : infectious)
	{
		for (auto* susceptiblePerson : susceptible)
		{
			infectiousPerson->Contact(*susceptiblePerson);
		}
	}
}

void DiseaseSpreadSimulation::Simulation::Print()
{
	// Only print once per hour
	//PrintEveryHour();
	// Only print once per day
	if (isNewDay)
	{
		PrintOncePerDay();
	}
}

void DiseaseSpreadSimulation::Simulation::PrintEveryHour()
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

void DiseaseSpreadSimulation::Simulation::PrintOncePerDay()
{
	for (size_t i = 0; i < communities.size(); i++)
	{
		auto& community = communities.at(i);

		fmt::print("\nCommunity #{} Day: {} Time : {} o'clock\n", i + 1, elapsedDays, time.GetTime());

		PrintPopulation(community.GetPopulation());
	}
}

void DiseaseSpreadSimulation::Simulation::PrintPopulation(const std::vector<Person>& population)
{
	size_t populationCount{0};
	size_t susceptible{0};
	size_t withDisease{0};
	size_t infectious{0};
	size_t deadPeople{0};
	size_t traveling{0};

	for (const auto& person : population)
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
			if (person.HasDisease())
			{
				withDisease++;
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
	fmt::print("With Disease: {}\n", withDisease);
	fmt::print("Infectious:   {}\n", infectious);
	fmt::print("Traveling:    {}\n", traveling);
	fmt::print("Have died:    {}\n", deadPeople);
}

bool DiseaseSpreadSimulation::Simulation::CheckForNewDay()
{
	if (time.GetElapsedDays() <= elapsedDays)
	{
		return false;
	}

	elapsedDays = time.GetElapsedDays();
	return true;
}

void DiseaseSpreadSimulation::Simulation::SetupEverything(uint32_t communityCount)
{
	DiseaseBuilder dbuilder;
	//diseases.push_back(dbuilder.CreateCorona());
	diseases.push_back(dbuilder.CreateDeadlyTestDisease());

	for (size_t i = 0; i < communityCount; i++)
	{
		communities.emplace_back(m_populationSize, country);

		InfectRandomPerson(&diseases.back(), communities.back().GetPopulation());
		SetupTravelInfecter(&diseases.back(), &communities.back());
	}

	stop = false;
	fmt::print("Setup complete\n");
}

void DiseaseSpreadSimulation::Simulation::InfectRandomPerson(const Disease* disease, std::vector<Person>& population)
{
	population.at(Random::RandomVectorIndex(population)).Contaminate(disease);
}

void DiseaseSpreadSimulation::Simulation::SetupTravelInfecter(const Disease* disease, Community* communitie)
{
	travelInfecter.Contaminate(disease);
	travelInfecter.SetCommunity(communitie);
	Home home{};
	travelInfecter.SetHome(&home);
	while (!travelInfecter.IsInfectious())
	{
		travelInfecter.Update(0, true, true);
	}
}
