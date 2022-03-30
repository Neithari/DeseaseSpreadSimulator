#pragma once

namespace Measure
{
	class MeasureTime
	{
	public:
		MeasureTime(std::string&& measureName);
		~MeasureTime();
	private:
		std::chrono::high_resolution_clock::time_point begin;
		const std::string measureName;
	};
}