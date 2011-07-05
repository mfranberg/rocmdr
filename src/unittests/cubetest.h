#include <rocmdrlib.h>

#include <cxxtest/TestSuite.h>

class CubeTestSuite : public CxxTest::TestSuite 
{
public:
  void testCube( void )
  {
      TS_ASSERT_EQUALS( cube(2), 8 );
  }
};
