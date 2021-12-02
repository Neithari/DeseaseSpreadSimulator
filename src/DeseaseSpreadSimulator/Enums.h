#pragma once

namespace DeseaseSpreadSimulation
{
	// Common
	enum class Country { USA, Germany };
	
	// Place related
	enum class Place_Type { Home, Supply, Workplace, HardwareStore, Morgue };
	
	// Person related
	enum class Seir_State { Susceptible, Exposed, Infectious, Recovered };
	// Age groups to differentiate between parts of the population from 0-9 years as first group up to > than 80 years as last group
	enum class Age_Group { UnderTen, UnderTwenty, UnderThirty, UnderFourty, UnderFifty, UnderSixty, UnderSeventy, UnderEighty, AboveEighty };
	enum class Sex { Female, Male };
}
