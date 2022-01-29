#pragma once
#include "Person/PersonPopulator.h"

namespace DeseaseSpreadSimulation
{
	class Community;

	class CommunityBuilder
	{
	public:
		CommunityBuilder() = default;

		Community CreateCommunity(const size_t populationSize, const Country country) const;
	};
}
