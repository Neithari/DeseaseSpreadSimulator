#pragma once

namespace DeseaseSpreadSimulation
{
	class TimeObserver
	{
	public:
		virtual void OnNewDay(Day currentDay) = 0;
		virtual ~TimeObserver() {};
	};
}
