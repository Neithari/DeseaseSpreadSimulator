#pragma once

namespace Measure
{
	class MeasureTime
	{
	public:
		explicit(false) MeasureTime(std::string measureName); // cppcheck-suppress noExplicitConstructor
		~MeasureTime();
	private:
		std::chrono::high_resolution_clock::time_point m_begin;
		const std::string m_measureName;
	};
} // namespace Measure
