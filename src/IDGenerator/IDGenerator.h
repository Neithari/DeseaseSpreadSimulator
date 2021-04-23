#pragma once
namespace IDGenerator
{
	// Get a ascending id separated for every type
	template <typename T>
	class IDGenerator
	{
	public:
		static unsigned int GetNextID()
		{
			static std::atomic_uint id{ 0 };
			return id++;
		}
	};
}
