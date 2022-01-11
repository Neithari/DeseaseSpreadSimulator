#include "pch.h"

namespace UnitTests {
    template <typename D, typename B>
    bool IsDerived(B* candidate)
    {
        D* derived = dynamic_cast<D*>(candidate);
        if (!derived)
        {
            return false;
        }
        return true;
    }

    class PersonStatesTest : public ::testing::Test
    {
    protected:
        std::unique_ptr<DeseaseSpreadSimulation::Home> home = std::make_unique<DeseaseSpreadSimulation::Home>();
        DeseaseSpreadSimulation::Community community;
        DeseaseSpreadSimulation::PersonBehavior behavior{ 1u,2u,1.f };
    };
    TEST_F(PersonStatesTest, FoodBuyTransitionTest)
    {
        using namespace DeseaseSpreadSimulation;

        behavior.foodBuyInterval = 0;
        Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, Sex::Male, behavior, community, home.get());
        
        auto state = std::make_unique<HomeState>(0, 0, Day::Monday);

        auto newState = state->HandleStateChange(person, 8);

        // Can't insert function call direct. There seems to be a bug.
        bool isDerived = IsDerived<HomeState, PersonStates>(state.get());
        ASSERT_TRUE(isDerived);

        isDerived = IsDerived<FoodBuyState, PersonStates>(newState.get());
        ASSERT_TRUE(isDerived);
    }
}
