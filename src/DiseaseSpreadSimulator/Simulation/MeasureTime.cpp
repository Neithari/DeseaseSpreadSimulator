#include "pch.h"
#include "Simulation/MeasureTime.h"

Measure::MeasureTime::MeasureTime(std::string&& measureName)
	: begin(std::chrono::high_resolution_clock::now()),
	  measureName(measureName)
{
}

Measure::MeasureTime::~MeasureTime()
{
	std::chrono::duration<double, std::milli> measurement = std::chrono::high_resolution_clock::now() - begin;
	fmt::print("{} took: {}ms\n", measureName, measurement.count());
}
