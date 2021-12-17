#include "pch.h"
#include "Person/PersonBehavior.h"

constexpr DeseaseSpreadSimulation::PersonBehavior::PersonBehavior(uint16_t foodBuyInterval, uint16_t hardwareBuyInterval, float acceptanceFactor)
	:
	foodBuyInterval(foodBuyInterval),
	hardwareBuyInterval(hardwareBuyInterval),
	acceptanceFactor(acceptanceFactor)
{
}
