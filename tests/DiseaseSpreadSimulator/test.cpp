#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>
#include "IDGenerator/IDGenerator.h"
#include "RandomNumbers.h"

namespace UnitTests
{
	constexpr auto testSize{10U};
	TEST(IDGeneratorTests, IDIncrementing) // cppcheck-suppress syntaxError
	{
		for (auto i = 0U; i < testSize; i++)
		{
			ASSERT_EQ(IDGenerator::IDGenerator<char>::GetNextID(), i);
		}
	}
	// No point in splitting the test
	// NOLINTNEXTLINE(*-cognitive-complexity)
	TEST(IDGeneratorTests, IndependentIDs)
	{
		for (auto i = 0U; i < testSize; i++)
		{
			ASSERT_EQ(IDGenerator::IDGenerator<int>::GetNextID(), i);
		}

		ASSERT_EQ(IDGenerator::IDGenerator<int>::GetNextID(), testSize);
		ASSERT_EQ(IDGenerator::IDGenerator<bool>::GetNextID(), 0U);

		for (auto i = 1U; i < testSize; i++)
		{
			ASSERT_EQ(IDGenerator::IDGenerator<bool>::GetNextID(), i);
		}

		ASSERT_EQ(IDGenerator::IDGenerator<std::string>::GetNextID(), 0U);

		for (auto i = 1U; i < testSize; i++)
		{
			ASSERT_EQ(IDGenerator::IDGenerator<std::string>::GetNextID(), i);
		}
	}
	TEST(IDGeneratorTests, UserDefinedClasses)
	{
		struct Foo
		{
			int f;
		};
		struct Bar
		{
			int b;
		};

		for (size_t i = 0; i < testSize; i++)
		{
			EXPECT_EQ(IDGenerator::IDGenerator<Foo>::GetNextID(), i);
			EXPECT_EQ(IDGenerator::IDGenerator<Bar>::GetNextID(), i);
		}
	}
	TEST(IDGeneratorTests, DerivedClasses)
	{
		class Base
		{
		protected:
			int b;
		};
		class Derived : public Base
		{
		public:
			int d;
		};
		for (size_t i = 0; i < testSize; i++)
		{
			EXPECT_EQ(IDGenerator::IDGenerator<Base>::GetNextID(), i);
			EXPECT_EQ(IDGenerator::IDGenerator<Derived>::GetNextID(), i);
		}
	}
	TEST(RandomNumbersTests, RandomVectorIndex)
	{
		std::vector<int> vec{};
		EXPECT_THROW(Random::RandomVectorIndex(vec), std::out_of_range);

		vec.push_back(1);
		for (size_t i = 0; i < testSize; i++)
		{
			EXPECT_EQ(Random::RandomVectorIndex(vec), 0);
		}

		vec.push_back(2);
		for (size_t i = 0; i < testSize; i++)
		{
			auto index = Random::RandomVectorIndex(vec);
			EXPECT_TRUE(index == 0 || index == 1);
		}
	}
	TEST(RandomNumbersTests, MapOneRangeToAnother)
	{
		constexpr auto min{1U};
		constexpr auto max{10U};
		for (auto i = min; i <= max; i++)
		{
			EXPECT_FLOAT_EQ(Random::MapOneRangeToAnother(static_cast<float>(i), static_cast<float>(min), static_cast<float>(max), 11.F, 20.F), static_cast<float>(i) + 10.F);
		}
	}
	TEST(RandomNumbersTests, MapRangeToPercent)
	{
		constexpr auto min{0U};
		constexpr auto max{10U};
		for (auto i = min; i <= max; i++)
		{
			EXPECT_FLOAT_EQ(Random::MapRangeToPercent(static_cast<float>(i), static_cast<float>(min), static_cast<float>(max)), static_cast<float>(i) * 0.1F);
		}
	}
} // namespace UnitTests
