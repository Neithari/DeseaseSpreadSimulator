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
        DeseaseSpreadSimulation::Home home;
        DeseaseSpreadSimulation::Community community;
        DeseaseSpreadSimulation::PersonBehavior behavior{ 1u,2u,1.f };
        uint16_t time{ 8 };

        std::string name = "a";
        int incubationPeriod = 1;
        int daysInfectious = 1;
        std::pair<int, int> deseaseDurationRange{ 2, 2 };
        std::vector<float> mortalityByAge{ 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f, 1.f };
        std::pair<int, int> daysTillDeathRange{ 1, 1 };
        DeseaseSpreadSimulation::Desease desease{ name, incubationPeriod, daysInfectious, deseaseDurationRange, mortalityByAge, daysTillDeathRange };
    };
    TEST_F(PersonStatesTest, FoodBuyTransitionTest)
    {
        using namespace DeseaseSpreadSimulation;

        Supply store;
        auto storeID = store.GetID();
        community.AddPlace(store);
        behavior.foodBuyInterval = 0;
        Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, Sex::Male, behavior, &community, &home);
        
        std::shared_ptr<PersonStates> state = std::make_shared<HomeState>(0, 0, Day::Monday);
        // Can't insert function call direct. There seems to be a bug.
        bool isDerived = IsDerived<HomeState, PersonStates>(state.get());
        ASSERT_TRUE(isDerived);

        auto newState = state->HandleStateChange(person, time);
        isDerived = IsDerived<FoodBuyState, PersonStates>(newState.get());
        ASSERT_TRUE(isDerived);

        newState->Enter(person);
        ASSERT_EQ(person.GetWhereabouts()->GetID(), storeID);

        auto testState = newState->HandleStateChange(person, time + 1);
        isDerived = IsDerived<HomeState, PersonStates>(testState.get());
        ASSERT_TRUE(isDerived);

        newState = state->HandleStateChange(person, time);
        isDerived = IsDerived<FoodBuyState, PersonStates>(newState.get());
        EXPECT_TRUE(isDerived);
        behavior.foodBuyInterval = 1;
        behavior.hardwareBuyInterval = 0;
        Person hardwarePerson(DeseaseSpreadSimulation::Age_Group::UnderTwenty, Sex::Male, behavior, &community, &home);
        testState = newState->HandleStateChange(hardwarePerson, 0);
        ASSERT_EQ(testState.get(), nullptr);
        testState = newState->HandleStateChange(hardwarePerson, time + 1);
        isDerived = IsDerived<HardwareBuyState, PersonStates>(testState.get());
        ASSERT_TRUE(isDerived);

        newState = state->HandleStateChange(person, time);
        isDerived = IsDerived<FoodBuyState, PersonStates>(newState.get());
        EXPECT_TRUE(isDerived);
        person.Contaminate(&desease);
        while (person.isAlive())
        {
            person.AdvanceDay();
        }
        testState = newState->HandleStateChange(person, time);
        isDerived = IsDerived<MorgueState, PersonStates>(testState.get());
        ASSERT_TRUE(isDerived);
    }
    TEST_F(PersonStatesTest, HardwareBuyTransitionTest)
    {
        using namespace DeseaseSpreadSimulation;

        HardwareStore store;
        auto storeID = store.GetID();
        community.AddPlace(store);
        behavior.hardwareBuyInterval = 0;
        Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, Sex::Male, behavior, &community, &home);

        std::shared_ptr<PersonStates> state = std::make_shared<HomeState>(0, 0, Day::Monday);
        // Can't insert function call direct. There seems to be a bug.
        bool isDerived = IsDerived<HomeState, PersonStates>(state.get());
        ASSERT_TRUE(isDerived);

        auto newState = state->HandleStateChange(person, time);
        isDerived = IsDerived<HardwareBuyState, PersonStates>(newState.get());
        ASSERT_TRUE(isDerived);

        newState->Enter(person);
        ASSERT_EQ(person.GetWhereabouts()->GetID(), storeID);

        auto testState = newState->HandleStateChange(person, time + 1);
        isDerived = IsDerived<HomeState, PersonStates>(testState.get());
        ASSERT_TRUE(isDerived);

        newState = state->HandleStateChange(person, time);
        isDerived = IsDerived<HardwareBuyState, PersonStates>(newState.get());
        EXPECT_TRUE(isDerived);
        behavior.foodBuyInterval = 0;
        behavior.hardwareBuyInterval = 1;
        Person foodPerson(DeseaseSpreadSimulation::Age_Group::UnderTwenty, Sex::Male, behavior, &community, &home);
        testState = newState->HandleStateChange(foodPerson, 0);
        ASSERT_EQ(testState.get(), nullptr);
        testState = newState->HandleStateChange(foodPerson, time + 1);
        isDerived = IsDerived<FoodBuyState, PersonStates>(testState.get());
        ASSERT_TRUE(isDerived);

        newState = state->HandleStateChange(person, time);
        isDerived = IsDerived<HardwareBuyState, PersonStates>(newState.get());
        EXPECT_TRUE(isDerived);
        person.Contaminate(&desease);
        while (person.isAlive())
        {
            person.AdvanceDay();
        }
        testState = newState->HandleStateChange(person, time);
        isDerived = IsDerived<MorgueState, PersonStates>(testState.get());
        ASSERT_TRUE(isDerived);
    }
    TEST_F(PersonStatesTest, WorkplaceTransitionTest)
    {
        using namespace DeseaseSpreadSimulation;

        Workplace work;
        auto workID = work.GetID();
        Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, Sex::Male, behavior, &community, &home);
        person.SetWorkplace(&work);
        community.AddPlace(work);

        std::shared_ptr<PersonStates> state = std::make_shared<HomeState>(0, 0, Day::Monday);
        // Can't insert function call direct. There seems to be a bug.
        bool isDerived = IsDerived<HomeState, PersonStates>(state.get());
        ASSERT_TRUE(isDerived);

        auto newState = state->HandleStateChange(person, time);
        isDerived = IsDerived<WorkState, PersonStates>(newState.get());
        ASSERT_TRUE(isDerived);

        newState->Enter(person);
        ASSERT_EQ(person.GetWhereabouts()->GetID(), workID);

        auto testState = newState->HandleStateChange(person, time);
        ASSERT_EQ(testState.get(), nullptr);
        testState = newState->HandleStateChange(person, 17);
        isDerived = IsDerived<HomeState, PersonStates>(testState.get());
        ASSERT_TRUE(isDerived);

        newState = state->HandleStateChange(person, time);
        isDerived = IsDerived<WorkState, PersonStates>(newState.get());
        EXPECT_TRUE(isDerived);
        person.Contaminate(&desease);
        while (person.isAlive())
        {
            person.AdvanceDay();
        }
        testState = newState->HandleStateChange(person, time);
        isDerived = IsDerived<MorgueState, PersonStates>(testState.get());
        ASSERT_TRUE(isDerived);
    }
    TEST_F(PersonStatesTest, SchoolTransitionTest)
    {
        using namespace DeseaseSpreadSimulation;

        School school;
        auto schoolID = school.GetID();
        Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, Sex::Male, behavior, &community, &home);
        person.SetSchool(&school);
        community.AddPlace(school);

        std::shared_ptr<PersonStates> state = std::make_shared<HomeState>(0, 0, Day::Monday);
        // Can't insert function call direct. There seems to be a bug.
        bool isDerived = IsDerived<HomeState, PersonStates>(state.get());
        ASSERT_TRUE(isDerived);

        auto newState = state->HandleStateChange(person, time);
        isDerived = IsDerived<SchoolState, PersonStates>(newState.get());
        ASSERT_TRUE(isDerived);

        newState->Enter(person);
        ASSERT_EQ(person.GetWhereabouts()->GetID(), schoolID);

        auto testState = newState->HandleStateChange(person, time);
        ASSERT_EQ(testState.get(), nullptr);
        testState = newState->HandleStateChange(person, 15);
        isDerived = IsDerived<HomeState, PersonStates>(testState.get());
        ASSERT_TRUE(isDerived);

        newState = state->HandleStateChange(person, time);
        isDerived = IsDerived<SchoolState, PersonStates>(newState.get());
        EXPECT_TRUE(isDerived);
        person.Contaminate(&desease);
        while (person.isAlive())
        {
            person.AdvanceDay();
        }
        testState = newState->HandleStateChange(person, time);
        isDerived = IsDerived<MorgueState, PersonStates>(testState.get());
        ASSERT_TRUE(isDerived);
    }
    TEST_F(PersonStatesTest, HomeTransitionTest)
    {
        using namespace DeseaseSpreadSimulation;

        Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, Sex::Male, behavior, &community, &home);

        std::unique_ptr<PersonStates> state = std::make_unique<HomeState>(0, 0, Day::Monday);
        // Can't insert function call direct. There seems to be a bug.
        bool isDerived = IsDerived<HomeState, PersonStates>(state.get());
        ASSERT_TRUE(isDerived);

        auto testState = state->HandleStateChange(person, 0);
        ASSERT_EQ(testState.get(), nullptr);

        state->Enter(person);
        ASSERT_EQ(person.GetWhereabouts()->GetID(), home.GetID());

        person.Contaminate(&desease);
        while (person.isAlive())
        {
            person.AdvanceDay();
        }
        testState = state->HandleStateChange(person, 0);
        isDerived = IsDerived<MorgueState, PersonStates>(testState.get());
        ASSERT_TRUE(isDerived);
    }
    TEST_F(PersonStatesTest, MorgueTest)
    {
        using namespace DeseaseSpreadSimulation;
        Morgue morgue;
        auto morgueID = morgue.GetID();
        community.AddPlace(morgue);
        Person person(DeseaseSpreadSimulation::Age_Group::UnderTwenty, Sex::Male, behavior, &community, &home);

        std::shared_ptr<PersonStates> state = std::make_shared<MorgueState>(0, 0, Day::Monday);
        // Can't insert function call direct. There seems to be a bug.
        bool isDerived = IsDerived<MorgueState, PersonStates>(state.get());
        ASSERT_TRUE(isDerived);

        auto testState = state->HandleStateChange(person, 0);
        ASSERT_EQ(testState.get(), nullptr);

        testState = state->HandleStateChange(person, 12);
        ASSERT_EQ(testState.get(), nullptr);

        testState = state->HandleStateChange(person, 24);
        ASSERT_EQ(testState.get(), nullptr);

        state->Enter(person);
        ASSERT_EQ(person.GetWhereabouts()->GetID(), morgueID);
    }
}
