#include <stdio.h>

extern "C"
{
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

#define TRACE_MSG(channel, msg, ...) { printf("[%s]", channel); printf(" "); printf(msg, __VA_ARGS__); printf("\n"); }
#define ERROR_TRACE(msg, ...) { TRACE_MSG("ERROR", msg, __VA_ARGS__); }

// Open various common libraries
void lua_open_libs(lua_State *l)
{
  luaopen_base(l);
  luaopen_math(l);
  luaopen_string(l);
  luaopen_table(l);
  luaopen_io(l);
  luaopen_os(l);
  luaopen_debug(l);
  luaopen_bit(l);
  luaopen_jit(l);
  luaopen_ffi(l);
}

// Loads a script and prints errors messages if any
bool loadScript(lua_State *l, const char* luaFile)
{
  int loadFileRes = luaL_loadfile(l, luaFile);
  if(loadFileRes != 0)
  {
    switch(loadFileRes)
    {
      case LUA_ERRSYNTAX:
	ERROR_TRACE("Syntax error in file %s", luaFile);
	break;
      case LUA_ERRMEM:
	ERROR_TRACE("Memory allocation error loading %s", luaFile);
	break;
      case LUA_ERRFILE:
	ERROR_TRACE("Cannot open/read file %s", luaFile);
	break;
      default:
	ERROR_TRACE("Unknown error occured when loading lua file %s", luaFile);
	break;
    }
    ERROR_TRACE("Lua loading errors: %s", lua_tostring(l, -1)); 
  }
  
  return loadFileRes == 0;
}

int main()
{
  lua_State *l = lua_open();
  lua_open_libs(l);
  
  char filename[128] = "data/first.lua";
  loadScript(l, filename);
  
  lua_pcall(l, 0,0,0);
  
  lua_close(l);
  
  return 0;
}
