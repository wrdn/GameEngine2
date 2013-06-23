#include <stdio.h>

extern "C"
{
  #include "lua.h"
  #include "lauxlib.h"
  #include "lualib.h"
}

int main()
{
  lua_State *l = lua_open();
  
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
  
  char filename[128] = "data/first.lua";
  
  luaL_dofile(l, filename);
  
  lua_close(l);
  
  return 0;
}
