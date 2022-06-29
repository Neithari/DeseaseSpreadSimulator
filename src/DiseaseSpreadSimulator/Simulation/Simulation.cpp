#include "Simulation/Simulation.h"
#include <algorithm>
#include <execution>
#include <cmath>
#include "fmt/core.h"
#include "Disease/DiseaseBuilder.h"
#include "RandomNumbers.h"

DiseaseSpreadSimulation::Simulation::Simulation(uint64_t populationSize, bool withPrint)
	: m_withPrint(withPrint),
	  m_populationSize(populationSize),
	  // log10(x) + 1 casted to integral will give us the digit count of x (1=1, 10=2, 100=3,...)
	  m_initialPopulationSizeDigitCount(static_cast<uint32_t>(std::log10(populationSize)) + 1U),
	  travelInfecter(Age_Group::UnderThirty, Sex::Male, PersonBehavior(100U, 100U, 1.F, 1.F), nullptr) // NOLINT: There is no benefit in named constants here
{
}
void DiseaseSpreadSimulation::Simulation::Run()
{
	SetupEverything(m_communityCount);

	while (!stop)
	{
		while (pause)
		{
			// Idle when pause is called until resumed
		}

		Update();
	}
}

void DiseaseSpreadSimulation::Simulation::RunForDays(uint32_t days)
{
	constexpr auto runNumber{1U};
	const auto runHours = days * 24U;
	SetupEverything(m_communityCount);

	for (auto hours = 0U; hours < runHours; hours++)
	{
		Update();
	}

	// Separate the output when we print during the simulation
	if (m_withPrint)
	{
		fmt::print("\n\n");
	}
	PrintRunResult(runNumber, days);
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
				std::shared_lock<std::shared_mutex> lockTravelInfecter(travelInfecterMutex);
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

void DiseaseSpreadSimulation::Simulation::Print() const
{
	// Only print once per hour
	//PrintEveryHour();
	// Only print once per day
	if (isNewDay)
	{
		PrintOncePerDay();
	}
}

void DiseaseSpreadSimulation::Simulation::PrintEveryHour() const
{
	for (const auto& community : communities)
	{

		fmt::print("\nCommunity id: {} Day: {} Time : {} o'clock\n", community.GetID(), elapsedDays, time.GetTime());

		PrintPopulation(community.GetPopulation());

		// Print public places
		const auto& places = community.GetPlaces();
		for (const auto& place : places.workplaces)
		{
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID() + 1, place.GetPersonCount());
		}
		for (const auto& place : places.schools)
		{
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID() + 1, place.GetPersonCount());
		}
		for (const auto& place : places.supplyStores)
		{
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID() + 1, place.GetPersonCount());
		}
		for (const auto& place : places.hardwareStores)
		{
			fmt::print("{} #{}: {} persons\n", Place::TypeToString(place.GetType()), place.GetID() + 1, place.GetPersonCount());
		}
	}
}

void DiseaseSpreadSimulation::Simulation::PrintOncePerDay() const
{
	for (const auto& community : communities)
	{
		fmt::print("\nCommunity id: {} Day: {} Time : {} o'clock\n", community.GetID(), elapsedDays, time.GetTime());

		PrintPopulation(community.GetPopulation());
	}
}

void DiseaseSpreadSimulation::Simulation::PrintPopulation(const std::vector<Person>& population) const
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

	fmt::print("Population:   {:>{}}\t\t", populationCount, m_initialPopulationSizeDigitCount);
	fmt::print("Susceptible: {:>{}}\n", susceptible, m_initialPopulationSizeDigitCount);
	fmt::print("With Disease: {:>{}}\t\t", withDisease, m_initialPopulationSizeDigitCount);
	fmt::print("Infectious:  {:>{}}\n", infectious, m_initialPopulationSizeDigitCount);
	fmt::print("Traveling:    {:>{}}\t\t", traveling, m_initialPopulationSizeDigitCount);
	fmt::print("Have died:   {:>{}}\n", deadPeople, m_initialPopulationSizeDigitCount);
}

void DiseaseSpreadSimulation::Simulation::PrintRunResult(const uint32_t runNumber, const uint32_t days) const
{
	// Containment measures
	// Starting population
	// Deaths
	// Survives
	// infection max
	// disease discovered by tests
	// Persons quarantined

	fmt::print("-------------------------------------------------------------------------------------\n");
	fmt::print("Simulation #{} simulated {} days and started with {} persons in {} communities.\n", runNumber, days, m_populationSize, m_communityCount);
	for (const auto& community : communities)
	{
		// Print mandates
		fmt::print("Community with id {}", community.GetID());
		
		const auto& containmentMeasures = community.ContainmentMeasures();
		if (!containmentMeasures.IsMaskMandate()
			&& !containmentMeasures.WorkingFormHome()
			&& !containmentMeasures.ShopsAreClosed()
			&& !containmentMeasures.IsLockdown())
		{
			fmt::print(" has no containment measures");
		}
		else
		{
			if (containmentMeasures.IsMaskMandate())
			{
				fmt::print(" has mask mandate");
			}
			if (containmentMeasures.WorkingFormHome())
			{
				fmt::print(" has home office mandate");
			}
			if (containmentMeasures.ShopsAreClosed())
			{
				fmt::print(" has shops are closed");
			}
			if (containmentMeasures.IsLockdown())
			{
				fmt::print(" has full lockdown");
			}
		}
		
		fmt::print("\nCurrent population status:\n");
		PrintPopulation(community.GetPopulation());

		fmt::print("Total infection count: {}\n", community.CurrentInfectionMax());
		fmt::print("Positive Tests: {}\t", community.NumberOfPositiveTests());
		fmt::print("Persons Quarantined: {}\n", community.NumberOfPersonsQuarantined());
	}
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
		communities.emplace_back(m_populationSize, m_country);

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
