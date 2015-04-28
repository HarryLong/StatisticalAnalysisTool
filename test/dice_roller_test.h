#include "../dice_roller.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class DiceRollerTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE( DiceRollerTest );
    CPPUNIT_TEST( testDiceRolls );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

    void testDiceRolls();

private:
    DiceRoller * m_dice_roller;
};

CPPUNIT_TEST_SUITE_REGISTRATION( DiceRollerTest );
