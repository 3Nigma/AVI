#ifndef _SETTINGS_HPP_
#define _SETTINGS_HPP_

#include <iostream>
#include <lua5.1/lua.hpp>
#include "exceptions.hpp"

class Settings{
public:
  static Settings &instance();
  void loadFromFile(const std::string &fileName) throw(LuaException);

private:
  Settings() throw(LuaMemAllocException);
  ~Settings();
  Settings(const Settings&) = default;
  Settings &operator=(const Settings&) = default;
protected:
  static Settings sSettings;
  lua_State *lstate;
};

#endif
