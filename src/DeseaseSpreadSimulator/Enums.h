#pragma once

namespace DeseaseSpreadSimulation
{
	// Common
	enum class Country { USA, Germany };
	enum class Day { Monday = 0, Tuesday,Wednesday, Thursday, Friday, Saturday, Sunday};

	// Place related
	enum class Place_Type { Home, Supply, Workplace, School, HardwareStore, Morgue, Travel };
	
	// Person related
	enum class Seir_State { Susceptible, Exposed, Infectious, Recovered };
	// Age groups to differentiate between parts of the population from 0-9 years as first group up to > than 80 years as last group
	enum class Age_Group { UnderTen, UnderTwenty, UnderThirty, UnderFourty, UnderFifty, UnderSixty, UnderSeventy, UnderEighty, AboveEighty };
	enum class Sex { Female, Male };
}
