#include "pch.h"
#include "Systems/MoveSystem.h"

void DeseaseSpreadSimulation::MoveSystem::AddCommunity(Community* community)
{
	m_communities.emplace_back(community);
}

void DeseaseSpreadSimulation::MoveSystem::Update()
{
	MovePersons();
}

void DeseaseSpreadSimulation::MoveSystem::MovePersons()
{
	for (auto community : m_communities)
	{
		auto& population = community->GetPopulation();

		for (auto& person : population)
		{
			// TODO: Check if it is already time to make a new move
			
			person->Move(GetDestination(person.get()));
		}
	}
}

DeseaseSpreadSimulation::Place* DeseaseSpreadSimulation::MoveSystem::GetDestination(Person* person)
{
	// TODO: Implement different behaviors
	// TODO: Get behavior and last visited places and decide where to move next
	person->GetHome(); //temporary
	return nullptr;
}
