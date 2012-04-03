#ifndef _SETTINGS_HPP_
#define _SETTINGS_HPP_

#include <iostream>
#include <map>
#include <lua5.1/lua.hpp>
#include "exceptions.hpp"
#include "gvalue.hpp"

class Settings{
public:
  static Settings &instance();
  void loadFromFile(const std::string &fileName) throw(LuaException);
  template<class T>
  T get(const std::string &key){
    return settings.find(key)->second;
  }

private:
  Settings() throw(LuaMemAllocException);
  ~Settings();
  Settings(const Settings&) = default;
  Settings &operator=(const Settings&) = default;

protected:
  static Settings sSettings;
  std::map<std::string, GSValue> settings;
  lua_State *lstate;
};

#endif
