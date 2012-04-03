#include <cxxtest/TestSuite.h>
#include "settings.hpp"
#include "exceptions.hpp"

class Settings_Tests : public CxxTest::TestSuite{
public:
  void testloadFromFile(void){
    TS_ASSERT_THROWS(Settings::instance().loadFromFile("does_not_exist.lua"), LuaFileException);
  }
};
