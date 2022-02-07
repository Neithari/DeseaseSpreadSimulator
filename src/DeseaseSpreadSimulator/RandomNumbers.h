#pragma once
#include <concepts>
#include <random>

namespace Random
{
	static std::random_device seed;
	static std::mt19937_64 generator(seed());

	template <typename T>
	static auto RandomVectorIndex(const std::vector<T>& vector)
	{
		// Prevent return of a negative index
		if (vector.empty())
		{
			throw(std::out_of_range("Vector is Empty!"));
		}

		using size_type = std::vector<T>::size_type;
		std::uniform_int_distribution<size_type> distribution((size_type)0, vector.size() - (size_type)1);

		return distribution(generator);
	};

	template <std::integral T>
	static auto UniformIntRange(T min, T max)
	{
		std::uniform_int_distribution<T> distribution(min, max);

		return distribution(generator);
	}

	template <std::floating_point T>
	static auto UniformFloatRange(T min, T max)
	{
		std::uniform_real_distribution<T> distribution(min, max);

		return distribution(generator);
	}

	// Return a random percentage between 0 and 1
	template <std::floating_point T>
	static auto Percent()
	{
		return UniformFloatRange((T)0.0, (T)1.0);
	}
}
