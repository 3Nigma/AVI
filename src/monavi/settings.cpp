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
  case LUA_ERRFILE: // cannot open/read the file
    throw LuaFileException("Unable to open/read the lua script file");
    break;
  case LUA_ERRMEM: // memory allocation error
    throw LuaMemAllocException("Unable to allocate space for the hosting of the instructions");
    break;
  case LUA_ERRSYNTAX: // syntax error during pre-compilation
    throw LuaExecCodeException("The file contained unrecognized lua statements");
    break;
  case 0: // all went ok
    break;
  default: // it shouldn't reach this point
    break;
  }

  int luaCallState = 0;
  std::string lerrormsg = "";
  
  if((luaCallState = lua_pcall(lstate, 0, 0 /* LUA_MULTRET */, 0)) != 0)
    lerrormsg = lua_tostring(lstate, -1);

  switch(luaCallState) {
  case LUA_ERRRUN:  // a runtime error
    throw LuaRuntimeException(std::string("A runtime error occured in the lua environment :") + lerrormsg);
    break;
  case LUA_ERRMEM:  // memory allocation error
    throw LuaMemAllocException("Unable to allocate space for instruction evaluation");
    break;
  case LUA_ERRERR:  // error while running the error handler function
    throw LuaErrorFuncException("The error handler function raised an error of its own");
    break;
  case 0:  // all went ok
    break;
  default:  // it shouldn't reach this point
    break;
  }

  lua_getglobal(lstate, "serial");
  //lua_getfield(lstate, -1, "parity");
  //if(luaL_checkint(lstate, -1) != 0)
  //  printf("%d\n", (int)lua_tonumber(lstate, -1));
  //printf("%s\n", lua_tostring(lstate, -1));
  //lua_getglobal(lstate, "serial");
  //lua_getfield(lstate, -1, "baud");
  //printf("%s\n", lua_tostring(lstate, -1));
  /* table is in the stack at index 't' */
  int t = lua_gettop(lstate);
  lua_pushnil(lstate);  /* first key */
  while (lua_next(lstate, t) != 0) {
    /* uses 'key' (at index -2) and 'value' (at index -1) */
    printf("%s - %s\n",
	   lua_typename(lstate, lua_type(lstate, -2)),
	   lua_typename(lstate, lua_type(lstate, -1)));
    /* removes 'value'; keeps 'key' for next iteration */
    lua_pop(lstate, 1);
  }
}
