#include "pch.h"

// Includes to be tested
#include "Disease/Disease.h"
#include "Disease/DiseaseBuilder.h"
#include "Person/Person.h"
#include "Simulation/TimeManager.h"
#include "Person/PersonPopulator.h"
#include "IDGenerator/IDGenerator.h"
#include "Places/Places.h"
#include "Places/Community.h"

namespace UnitTests {
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
        private:
            int d;
        };
        for (size_t i = 0; i < 10; i++)
        {
            EXPECT_EQ(IDGenerator::IDGenerator<Base>::GetNextID(), i);
            EXPECT_EQ(IDGenerator::IDGenerator<Derived>::GetNextID(), i);
        }
    }
}
