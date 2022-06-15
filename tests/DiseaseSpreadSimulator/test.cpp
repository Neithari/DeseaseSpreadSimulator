#include <gtest/gtest.h>
#include <vector>
#include <stdexcept>
#include "IDGenerator/IDGenerator.h"
#include "RandomNumbers.h"

namespace UnitTests
{
	TEST(IDGeneratorTests, IDIncrementing)
	{
		for (size_t i = 0; i < 10; i++)
		{
			ASSERT_EQ(IDGenerator::IDGenerator<char>::GetNextID(), i);
		}
	}
	TEST(IDGeneratorTests, IndependentIDs)
	{
		for (size_t i = 0; i < 10; i++)
		{
			ASSERT_EQ(IDGenerator::IDGenerator<int>::GetNextID(), i);
		}

		ASSERT_EQ(IDGenerator::IDGenerator<int>::GetNextID(), 10);
		ASSERT_EQ(IDGenerator::IDGenerator<bool>::GetNextID(), 0);

		for (size_t i = 1; i < 10; i++)
		{
			ASSERT_EQ(IDGenerator::IDGenerator<bool>::GetNextID(), i);
		}

		ASSERT_EQ(IDGenerator::IDGenerator<std::string>::GetNextID(), 0);

		for (size_t i = 1; i < 10; i++)
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

		for (size_t i = 0; i < 10; i++)
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
		for (size_t i = 0; i < 10; i++)
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
		for (size_t i = 0; i < 10; i++)
		{
			EXPECT_EQ(Random::RandomVectorIndex(vec), 0);
		}

		vec.push_back(2);
		for (size_t i = 0; i < 10; i++)
		{
			auto index = Random::RandomVectorIndex(vec);
			EXPECT_TRUE(index == 0 || index == 1);
		}
	}
	TEST(RandomNumbersTests, MapOneRangeToAnother)
	{
		float min{1};
		float max{10};
		for (float i = min; i <= max; i++)
		{
			EXPECT_FLOAT_EQ(Random::MapOneRangeToAnother(i, min, max, 11.f, 20.f), i + 10.f);
		}
	}
	TEST(RandomNumbersTests, MapRangeToPercent)
	{
		float min{0};
		float max{10};
		for (float i = min; i <= max; i++)
		{
			EXPECT_FLOAT_EQ(Random::MapRangeToPercent(i, min, max), i * 0.1f);
		}
	}
} // namespace UnitTests
