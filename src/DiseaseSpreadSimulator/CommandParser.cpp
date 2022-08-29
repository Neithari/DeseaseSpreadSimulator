#include "CommandParser.h"
#include <algorithm>
#include <cstdint>

DiseaseSpreadSimulation::CommandParser::CommandParser(int argc, char* argv[]) // NOLINT: We need the c-style array here
{
	for (int i = 0; i < argc; i++)
	{
		commands.emplace_back(argv[i]); // NOLINT: We need the pointer arithmetic here
	}
}

uint64_t DiseaseSpreadSimulation::CommandParser::GetPopulationSize() const
{
	static constexpr auto command{"-p"};
	if (CommandExist(command))
	{
		return static_cast<uint64_t>(std::stoull(GetCommandOption(command)));
	}

	return populationSize;
}

uint32_t DiseaseSpreadSimulation::CommandParser::GetDaysToRun() const
{
	static constexpr auto command{"-d"};
	if (CommandExist(command))
	{
		return static_cast<uint32_t>(std::stoul(GetCommandOption(command)));
	}

	return daysToRun;
}

uint32_t DiseaseSpreadSimulation::CommandParser::GetNumberOfRuns() const
{
	static constexpr auto command{"-n"};
	if (CommandExist(command))
	{
		return static_cast<uint32_t>(std::stoul(GetCommandOption(command)));
	}

	return numberOfRuns;
}

bool DiseaseSpreadSimulation::CommandParser::GetWithPrint() const
{
	return CommandExist("-o");
}

const std::string& DiseaseSpreadSimulation::CommandParser::GetDiseaseFilename() const
{
	static constexpr auto command{"-f"};
	if (CommandExist(command))
	{
		return GetCommandOption(command);
	}

	static const std::string emptyString{};
	return emptyString;
}

DiseaseSpreadSimulation::Country DiseaseSpreadSimulation::CommandParser::GetCountry() const
{
	static constexpr auto command{"-c"};
	if (CommandExist(command))
	{
		const auto& country = GetCommandOption(command);
		if (country == "USA")
		{
			return Country::USA;
		}
		if (country == "Germany")
		{
			return Country::Germany;
		}
	}

	return Country::USA;
}

bool DiseaseSpreadSimulation::CommandParser::CommandExist(std::string_view command) const
{
	return std::find(commands.begin(), commands.end(), command) != commands.end();
}

const std::string& DiseaseSpreadSimulation::CommandParser::GetCommandOption(std::string_view command) const
{
	auto commandIt = std::find(commands.begin(), commands.end(), command);
	
	if (commandIt != commands.end() && ++commandIt != commands.end())
	{
		return *commandIt;
	}

	static const std::string emptyString{};
	return emptyString;
}
