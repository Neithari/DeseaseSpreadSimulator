#pragma once

namespace DeseaseSpreadSimulation
{
	class MoveSystem
	{
	public:
		MoveSystem() = default;

		void AddCommunity(Community& community);
		void Update();

	private:
		void MovePersons();
		Place* GetDestination(Person* person);

	private:
		std::vector<Community&> m_communities;
	};
}
