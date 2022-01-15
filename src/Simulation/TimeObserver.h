#pragma once

namespace DeseaseSpreadSimulation
{
	class TimeObserver
	{
	public:
		virtual void OnNewDay(Day newDay) = 0;
		virtual ~TimeObserver() {};
	};
}
