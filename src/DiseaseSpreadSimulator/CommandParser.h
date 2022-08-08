#include <vector>
#include <string>
#include <string_view>

namespace DiseaseSpreadSimulation
{
	class CommandParser
	{
	public:
		CommandParser(int argc, char* argv[]);

		// Will return default or command line argument provided population size
		[[nodiscard]] uint64_t GetPopulationSize() const;
		// Will return default or command line argument provided days to run
		[[nodiscard]] uint32_t GetDaysToRun() const;
		// Will return default or command line argument provided number of runs
		[[nodiscard]] uint32_t GetNumberOfRuns() const;
		// Will return false default or true if command line argument is provided
		[[nodiscard]] bool GetWithPrint() const;
		
		// Filename can be empty
		[[nodiscard]] const std::string& GetDiseaseFilename() const;

		[[nodiscard]] bool CommandExist(std::string_view command) const;
		[[nodiscard]] const std::string& GetCommandOption(std::string_view command) const;

	private:
		std::vector<std::string> commands;

		// Defaults
		static constexpr uint64_t populationSize{1000U};
		static constexpr uint32_t daysToRun{365U};
		static constexpr uint32_t numberOfRuns{1U};
	};
} // namespace DiseaseSpreadSimulation
