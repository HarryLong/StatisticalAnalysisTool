#include "dice_roller_test.h"

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
    int count [] = {0,0,0,0,0,0};
    for(int i (0); i < 6000; i++ )
    {
        count[m_dice_roller->generate()]++;
    }

    CPPUNIT_ASSERT( count[0] > 990 && count[0] < 990);
    CPPUNIT_ASSERT( count[1] > 990 && count[1] < 990);
    CPPUNIT_ASSERT( count[2] > 990 && count[2] < 990);
    CPPUNIT_ASSERT( count[3] > 990 && count[3] < 990);
    CPPUNIT_ASSERT( count[4] > 990 && count[4] < 990);
    CPPUNIT_ASSERT( count[5] > 990 && count[5] < 990);
}
