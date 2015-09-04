#include "dice_roller_test.h"
#include "../utils/dice_roller.h"
#include <string>

void DiceRollerTest::setUp()
{
    m_dice_roller = new DiceRoller(1,6);
}

void DiceRollerTest::tearDown()
{
    delete m_dice_roller;
}

void DiceRollerTest::testDiceRolls()
{
    double ratios [] = {.0,.0,.0,.0,.0,.0};
    for(int i (0); i < 20000; i++ )
        ratios[m_dice_roller->generate()-1] += (1.0/20000);

    double expected (1.0/6);
    double delta(0.01);

    for(int i (0); i < 6; i++)
    {
        // "Value " + std::to_string(i+1) + " occured with frequency " + std::to_string((ratios[i]*100)) + "%. Required: " +
        // std::to_string((expected*100)) + "% (delta: " + std::to_string((delta*100)) + ")",
        CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, ratios[i], delta);
    }
}
