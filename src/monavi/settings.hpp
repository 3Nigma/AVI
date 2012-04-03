#ifndef _SETTINGS_HPP_
#define _SETTINGS_HPP_

#include <iostream>
#include <map>
#include <lua5.1/lua.hpp>
#include "exceptions.hpp"

class GValue {
public:
  GValue(const std::string &val) : mVal(val) {};
  void setValue(const std::string &newVal) {
    mVal = newVal;
  }
  bool operator==(const GValue &rhs) const {
    return mVal == rhs.mVal;
  }
  operator int() const {
    return stoi(mVal);
  }
  operator std::string() const {
    return mVal;
  }
  operator double() const {
    return stod(mVal);
  }
  operator float() const {
    return stof(mVal);
  }
private:
  std::string mVal;
};

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
  std::map<std::string, GValue> settings;
  lua_State *lstate;
};

#endif
