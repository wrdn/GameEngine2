#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <string>

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

int wildcmp(const char *wild, const char *string) {
  // Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return 0;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return 1;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}

// searchTerm is the wild card string passed to wildcmp e.g. for all files ending in .lua in dir/ pass "*.lua" as the searchTerm
// To disable recursion, just set maxLevel=0
int findFiles(char* searchTerm, const char* directory, std::vector<std::string> &outputVec, unsigned int maxLevel=-1, unsigned int level=0)
{
  std::vector<char*> subDirs;
  
  int numFound = 0;
  DIR *dir = opendir(directory);
  if(dir)
  {
    struct dirent *ent;
    while((ent = readdir(dir)) != NULL)
    {
      if(!ent->d_name) continue;
      
      if(ent->d_type == DT_REG) // regular files
      {
	if(wildcmp(searchTerm, ent->d_name))
	{
	  std::string outputFileName = std::string(directory);
	  if(outputFileName[outputFileName.length()-1] != '/') outputFileName += "/";
	  outputFileName += ent->d_name;
	  
	  outputVec.push_back(outputFileName);
	  ++numFound;
	}
      }
      else if(ent->d_type == DT_DIR && level+1 <= maxLevel) // recurse down multiple levels (to to max depth)
      {
	if(strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
	{
	  subDirs.push_back(ent->d_name); // add the directory to the list of directories to search
	}
      }
    }
    
    // after we've finished with all of the files in the current directory, we'll search the sub-directories, the next level down
    for(int i=0;i<subDirs.size();++i)
    {
      std::string nextDir = std::string(directory) + "/" + std::string(subDirs[i]);
      numFound += findFiles(searchTerm, nextDir.c_str(), outputVec, maxLevel, level+1);
    }
    
    closedir(dir);
  }
  return numFound;
}

void loadAndRunScript(lua_State *l, const char* filename)
{
  printf("Running script %s\n", filename);
  loadScript(l, filename);
  lua_pcall(l,0,0,0);
}

int main()
{
  std::vector<std::string> luaFiles;
  int numLuaFiles = findFiles("*.lua", "data", luaFiles);
  
  printf("Found %i lua file(s)\n", numLuaFiles);
  
  for(int i=0;i<luaFiles.size();++i)
    printf("Found %s\n", luaFiles[i].c_str());
  
  lua_State *l = lua_open();
  lua_open_libs(l);
  
  printf("Executing scripts...\n");
  
  for(int i=0;i<luaFiles.size();++i)
  {
    loadAndRunScript(l, luaFiles[i].c_str());
  }
  
  lua_close(l);
  
  return 0;
}
