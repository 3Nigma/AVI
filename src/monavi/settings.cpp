#include "settings.hpp"

Settings Settings::sSettings;

Settings::Settings()
  throw(LuaMemAllocException) {
  lstate = luaL_newstate();
  if(NULL == lstate) throw LuaMemAllocException("Unable to allocate space for a lua state construction");
  //luaL_openlibs(lstate);
}

Settings::~Settings() {
  lua_close(lstate);
}

Settings &Settings::instance() {
  return sSettings;
}

void Settings::loadFromFile(const std::string &fileName) 
  throw(LuaException) {
  int luaFileState = luaL_loadfile(lstate, fileName.c_str());
  
  switch(luaFileState) {
  case LUA_ERRFILE:  /* cannot open/read the file */
    throw LuaFileException("Unable to open/read the lua script file");
    break;
  case LUA_ERRMEM:  /* memory allocation error */
    throw LuaMemAllocException("Unable to allocate space for the hosting of the instructions");
    break;
  case LUA_ERRSYNTAX:  /* syntax error during pre-compilation */
    throw LuaExecCodeException("The file contained unrecognized lua statements");
    break;
  case 0:  /* all went ok */
    break;
  default: /* it shouldn't reach this point */
    break;
  }

  int luaCallState = 0;
  std::string lerrormsg = "";
  
  if((luaCallState = lua_pcall(lstate, 0, 0, 0)) != 0)
    lerrormsg = lua_tostring(lstate, -1);

  switch(luaCallState) {
  case LUA_ERRRUN:  /* a runtime error */
    throw LuaRuntimeException(std::string("A runtime error occured in the lua environment :") + lerrormsg);
    break;
  case LUA_ERRMEM:  /* memory allocation error */
    throw LuaMemAllocException("Unable to allocate space for instruction evaluation");
    break;
  case LUA_ERRERR:  /* error while running the error handler function */
    throw LuaErrorFuncException("The error handler function raised an error of its own");
    break;
  case 0:  /* all went ok */
    break;
  default:  /* it shouldn't reach this point */
    break;
  }

  lua_pushnil(lstate);
  while (lua_next(lstate, LUA_GLOBALSINDEX) != 0) {
    int key_type = lua_type(lstate, -2);
    int value_type = lua_type(lstate, -1);

    if (key_type != LUA_TSTRING) {
      lua_pop(lstate, 1);
      continue;
    }

    if (value_type != LUA_TNUMBER &&
	value_type != LUA_TBOOLEAN &&
	value_type != LUA_TSTRING) {
      lua_pop(lstate, 1);
      continue;
    }

    std::string key_string = lua_tostring(lstate, -2);

    if (!key_string.size()) {
      lua_pop(lstate, 1);
      continue;
    }
    if (key_string[0] == '_') {
      lua_pop(lstate, 1);
      continue;
    }

    std::string value_string = lua_tostring(lstate, -1);
    
    settings.insert({key_string, GSValue(value_string)});

    lua_pop(lstate, 1);
  }
}
