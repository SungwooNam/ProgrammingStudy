#ifndef __PROGRAMMING_LUA_COMMON_H__
#define __PROGRAMMING_LUA_COMMON_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

namespace ProgrammingLua
{

void stackDump( lua_State *L );
void error( lua_State* L, const char *fmt, ... );

}

#endif
