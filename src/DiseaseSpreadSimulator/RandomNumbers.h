#pragma once
#include <vector>
#include <concepts>
#include <random>
#include <stdexcept>

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

		typedef typename std::vector<T>::size_type size_type;
		std::uniform_int_distribution<size_type> distribution(static_cast<size_type>(0), vector.size() - static_cast<size_type>(1));

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
		return UniformFloatRange(static_cast<T>(0.0), static_cast<T>(1.0));
	}

	template <typename T, typename = typename std::enable_if_t<std::is_floating_point<T>::value>>
	static T MapOneRangeToAnother(T value, T fromRangeMin, T fromRangeMax, T toRangeMin, T toRangeMax)
	{
		// When from range consists of only one number return the max of the to range
		// This will prevent division by 0
		if (fromRangeMin == fromRangeMax)
		{
			return toRangeMax;
		}
		T slope = static_cast<T>(1) * (toRangeMax - toRangeMin) / (fromRangeMax - fromRangeMin);
		return toRangeMin + slope * (value - fromRangeMin);
	}

	template <typename T, typename = typename std::enable_if_t<std::is_floating_point<T>::value>>
	static T MapRangeToPercent(T value, T fromRangeMin, T fromRangeMax)
	{
		return MapOneRangeToAnother(value, fromRangeMin, fromRangeMax, static_cast<T>(0), static_cast<T>(1));
	}
} // namespace Random
