#pragma once

namespace DeseaseSpreadSimulation
{
	class TimeObserver
	{
	public:
		virtual void OnNewDay() = 0;
		virtual ~TimeObserver() {};
	};
}
