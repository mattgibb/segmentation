#include <cxxtest/TestSuite.h>
class TestReadMeshWriteVtk : public CxxTest::TestSuite
{
public:
  void TestOnePlusOneEqualsTwo()
  {
    int some_number = 1 + 1;
    TS_ASSERT_DELTA(some_number, 2);
    
  }
};