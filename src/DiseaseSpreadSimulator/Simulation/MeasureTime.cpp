#include "Simulation/MeasureTime.h"

Measure::MeasureTime::MeasureTime(std::string measureName)
	: m_begin(std::chrono::high_resolution_clock::now()),
	  m_measureName(std::move(measureName))
{
}

Measure::MeasureTime::~MeasureTime()
{
	std::chrono::duration<double, std::milli> measurement = std::chrono::high_resolution_clock::now() - m_begin;
	fmt::print("{} took: {}ms\n", m_measureName, measurement.count());
}
