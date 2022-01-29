#pragma once
namespace IDGenerator
{
	// Get a ascending id separated for every type
	template <typename T>
	class IDGenerator
	{
	public:
		static uint32_t GetNextID()
		{
			static std::atomic<uint32_t> id{ 0 };
			return id++;
		}
	};
}
