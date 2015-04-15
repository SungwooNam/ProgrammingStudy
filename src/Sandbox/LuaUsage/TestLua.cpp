#include <boost/test/unit_test.hpp>

#include "LuaCommon.h"

using namespace std;

namespace ProgrammingLua
{

void stackDump( lua_State *L )
{
    int i;
    int top = lua_gettop( L );
    for( i = 1; i <= top; i++ ) {
        int t = lua_type(L,i);
        switch( t ) {
            case LUA_TSTRING:
                printf( "'%s'", lua_tostring(L,i));
                break;

            case LUA_TBOOLEAN:
                printf( lua_toboolean(L,i) ? "true" : "false" );
                break;

            case LUA_TNUMBER:
                printf( "%g", lua_tonumber( L, i ) );
                break;

            default:
                printf( "%s", lua_typename(L,t) );
                break;

        }

        printf( " " );
    }
    printf("\n");
}

void error( lua_State* L, const char *fmt, ... )
{
    va_list argp;
    va_start(argp, fmt );
    vfprintf( stderr, fmt, argp );
    va_end(argp);

    lua_close( L );
    exit( EXIT_FAILURE );
}

}

namespace ProgrammingLua
{

int SimpleLineInterpreter( const char **lines ) 
{
    int error;
    lua_State *L = luaL_newstate();
    luaL_openlibs( L );

    while( *lines != NULL ) {
        error = luaL_loadbuffer( L, *lines, strlen(*lines), "line") ||
                lua_pcall( L, 0, 0, 0 );
        if( error ) { 
            fprintf( stderr, "%s", lua_tostring( L, -1 ));
            lua_pop(L,1);
        }

        lines++;
    }

    lua_close( L );
    return 0;
}

}


BOOST_AUTO_TEST_CASE( TestSimpleStandalone )
{  
    using namespace ProgrammingLua;

    const char *script [] = {
        "print(\"Hello World\") \n",
        NULL
    };
    SimpleLineInterpreter( script );
}


BOOST_AUTO_TEST_CASE( TestStackManipulation )
{  
    using namespace ProgrammingLua;

    lua_State *L = luaL_newstate();

    lua_pushboolean(L,1 );
    lua_pushnumber(L,10);
    lua_pushnil( L);
    lua_pushstring( L, "Hello" );

    stackDump(L);

    lua_pushvalue(L, -4 ); stackDump( L );

    lua_replace( L, 3 ); stackDump( L ); 

    lua_settop( L, 6 ); stackDump( L );

    lua_remove( L, -3 ); stackDump( L );
    
    lua_settop( L, -5 ); stackDump(L);

    lua_close( L );
}


namespace ProgrammingLua
{

void load( lua_State* L, const char *filename, int *w, int *h )
{
    if( luaL_loadfile( L, filename ) || 
        lua_pcall( L, 0, 0, 0 ) )
    {
        error( L, "cannot run config file : %s", lua_tostring(L,-1) );
    }

    lua_getglobal(L, "width");
    lua_getglobal(L, "height");
    if( !lua_isnumber(L,-2))
        error( L, "'width' should be a number\n" );
    if( !lua_isnumber(L,-1))
        error( L, "'height' should be a number\n" );

    *w = lua_tointeger( L, -2 );
    *h = lua_tointeger( L, -1 );
}

}

BOOST_AUTO_TEST_CASE( TestLuaAsConfigFile )
{  
    using namespace ProgrammingLua;

    lua_State *L = luaL_newstate();

    {
        FILE *fp = fopen( "test.lua", "wt" );
        fprintf( fp, "width = 200\n" );
        fprintf( fp, "height = 300\n" );
        fclose( fp );
    }

    int width = 0, height = 0;
    load( L, "test.lua", &width, &height );

    BOOST_CHECK_EQUAL( 200, width );
    BOOST_CHECK_EQUAL( 300, height );

    lua_close( L );
}

namespace ProgrammingLua
{

#define MAX_COLOR 255

int getfield( lua_State* L, const char *key )
{
    int result;
    lua_pushstring( L, key );
    lua_gettable( L, -2 );
    if( !lua_isnumber(L,-1))
    {
        error( L, "invalid component in background color" );
    }
    result = (int) ( lua_tonumber( L, -1 ) * MAX_COLOR );
    lua_pop( L , 1 );
    return result;
}

}

BOOST_AUTO_TEST_CASE( TestTableRead )
{  
    using namespace ProgrammingLua;

    {
        FILE *fp = fopen( "test.lua", "wt" );
        fprintf( fp, "background = { r = 0.3, g = 0.1, b = 0 }\n" );
        fclose( fp );
    }

    lua_State *L = luaL_newstate();

    if( luaL_loadfile( L, "test.lua" ) || 
        lua_pcall( L, 0, 0, 0 ) )
    {
        error( L, "cannot run config file : %s", lua_tostring(L,-1) );
    }

    {
        lua_getglobal( L, "background" );
        BOOST_CHECK( lua_istable(L,-1) );

        int red = getfield( L, "r" );
        int green = getfield( L, "g" );
        int blue = getfield( L, "b" );

        BOOST_CHECK_EQUAL( (int)(0.3*255), red );
        BOOST_CHECK_EQUAL( (int)(0.1*255), green );
        BOOST_CHECK_EQUAL( 0*255, blue );

        lua_pop( L, 1 );
    }

    {
        lua_getglobal( L, "background" );

        lua_getfield( L, -1, "r" );
        BOOST_CHECK_EQUAL( 0.3, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        lua_getfield( L, -1, "g" );
        BOOST_CHECK_EQUAL( 0.1, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        lua_getfield( L, -1, "b" );
        BOOST_CHECK_EQUAL( 0.0, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        lua_pop( L, 1 );
    }

    lua_close( L );
}

namespace ProgrammingLua
{

struct ColorTable {
    char *name;
    unsigned char red, green, blue;
} colortable[] = {
    {"WHITE", MAX_COLOR, MAX_COLOR, MAX_COLOR},
    {"RED", MAX_COLOR, 0, 0},
    {"GREEN", 0, MAX_COLOR, 0},
    {"BLUE", 0, 0, MAX_COLOR},
    {NULL, 0, 0, 0} /* sentinel */
};

void setfield (lua_State *L, const char *index, int value) {
    lua_pushnumber(L, (double)value/MAX_COLOR);
    lua_setfield(L, -2, index);
}

void setcolor (lua_State *L, struct ColorTable *ct) {
    lua_newtable(L); /* creates a table */
    setfield(L, "r", ct->red); /* table.r = ct->r */
    setfield(L, "g", ct->green); /* table.g = ct->g */
    setfield(L, "b", ct->blue); /* table.b = ct->b */
    lua_setglobal(L, ct->name); /* 'name' = table */
}

}

BOOST_AUTO_TEST_CASE( TestTableSetByC )
{  
    using namespace ProgrammingLua;

    {
        FILE *fp = fopen( "test.lua", "wt" );
        fprintf( fp, "background = BLUE\n" );
        fclose( fp );
    }

    lua_State *L = luaL_newstate();

    {
        int i = 0;
        while (colortable[i].name != NULL)
            setcolor(L, &colortable[i++]);
    }

    if( luaL_loadfile( L, "test.lua" ) || 
        lua_pcall( L, 0, 0, 0 ) )
    {
        error( L, "cannot run config file : %s", lua_tostring(L,-1) );
    }

    {
        lua_getglobal( L, "background" );

        lua_getfield( L, -1, "r" );
        BOOST_CHECK_EQUAL( 0, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        lua_getfield( L, -1, "g" );
        BOOST_CHECK_EQUAL( 0, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        lua_getfield( L, -1, "b" );
        BOOST_CHECK_EQUAL( 1.0, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        lua_pop( L, 1 );
    }

    lua_close( L );
}

namespace ProgrammingLua
{

/* call a function 'f' defined in Lua */
double callLuaFunction(lua_State *L, double x, double y) {
    double z;
    /* push functions and arguments */
    lua_getglobal(L, "f"); /* function to be called */
    lua_pushnumber(L, x); /* push 1st argument */
    lua_pushnumber(L, y); /* push 2nd argument */
    /* do the call (2 arguments, 1 result) */
    if (lua_pcall(L, 2, 1, 0) != 0)
        error(L, "error running function 'f': %s", lua_tostring(L, -1));
    /* retrieve result */
    if (!lua_isnumber(L, -1))
        error(L, "function 'f' must return a number");
    z = lua_tonumber(L, -1);
    lua_pop(L, 1); /* pop returned value */
    return z;
}


#include <stdarg.h>

void call_va ( lua_State *L, const char *func, const char *sig, ...) 
{
    va_list vl;
    int narg, nres; /* number of arguments and results */
    va_start(vl, sig);
    lua_getglobal(L, func); /* push function */

    for (narg = 0; *sig; narg++) { /* repeat for each argument */
        /* check stack space */
        luaL_checkstack(L, 1, "too many arguments");
        switch (*sig++) {
        case 'd': /* double argument */
            lua_pushnumber(L, va_arg(vl, double));
            break;
        case 'i': /* int argument */
            lua_pushinteger(L, va_arg(vl, int));
            break;
        case 's': /* string argument */
            lua_pushstring(L, va_arg(vl, char *));
            break;
        case '>': /* end of arguments */
            goto endargs;
        default:
            error(L, "invalid option (%c)", *(sig - 1));
        }
    }
    endargs:

    nres = strlen(sig); /* number of expected results */
    /* do the call */
    if (lua_pcall(L, narg, nres, 0) != 0) /* do the call */
        error(L, "error calling '%s': %s", func, lua_tostring(L, -1));
    
    nres = -nres; /* stack index of first result */
    while (*sig) { /* repeat for each result */
        switch (*sig++) {
        case 'd': /* double result */
            if (!lua_isnumber(L, nres))
                error(L, "wrong result type");
            *va_arg(vl, double *) = lua_tonumber(L, nres);
            break;
        case 'i': /* int result */
            if (!lua_isnumber(L, nres))
                error(L, "wrong result type");
            *va_arg(vl, int *) = lua_tointeger(L, nres);
            break;
        case 's': /* string result */
            if (!lua_isstring(L, nres))
                error(L, "wrong result type");
            *va_arg(vl, const char **) = lua_tostring(L, nres);
            break;
        default:
            error(L, "invalid option (%c)", *(sig - 1));
        }
        nres++;
    }
    
    va_end(vl);
}

}

BOOST_AUTO_TEST_CASE( TestCallingLuaFunctions )
{  
    using namespace ProgrammingLua;

    {
        FILE *fp = fopen( "test.lua", "wt" );
        fprintf( fp, "function f ( x, y ) \n" );
        fprintf( fp, "  return ( x^2 * math.sin(y))/(1-x) \n" );
        fprintf( fp, "end \n" );
        fclose( fp );
    }

    lua_State *L = luaL_newstate();

    luaopen_math(L);

    if( luaL_loadfile( L, "test.lua" ) || 
        lua_pcall( L, 0, 0, 0 ) )
    {
        error( L, "cannot run config file : %s", lua_tostring(L,-1) );
    }

    {
        double result = callLuaFunction( L, 0.5, 0.5 );
    }

    {
        double z = 0;
        call_va( L, "f", "dd>d", 0.5, 0.5, &z );
    }

    lua_close( L );
}

namespace ProgrammingLua
{

static int l_sin (lua_State *L) 
{
    double d = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(d));
    return 1; /* number of results */
}

}

BOOST_AUTO_TEST_CASE( TestCFunctions )
{  
    using namespace ProgrammingLua;

    {
        FILE *fp = fopen( "test.lua", "wt" );
        fprintf( fp, "print( mysin( 0.1 ) ) \n" );
        fclose( fp );
    }

    lua_State *L = luaL_newstate();

    luaL_openlibs( L );

    lua_pushcfunction( L, l_sin );
    lua_setglobal( L, "mysin" );

    if( luaL_loadfile( L, "test.lua" ) || 
        lua_pcall( L, 0, 0, 0 ) )
    {
        error( L, "cannot run config file : %s", lua_tostring(L,-1) );
    }

    lua_close( L );
}

namespace ProgrammingLua
{

static int l_map(lua_State *L) 
{
    int i, n;
    /* 1st argument must be a table (t) */
    luaL_checktype(L, 1, LUA_TTABLE);
    /* 2nd argument must be a function (f) */
    luaL_checktype(L, 2, LUA_TFUNCTION);
    n = lua_objlen(L, 1); /* get size of table */
    for (i = 1; i <= n; i++) {
        lua_pushvalue(L, 2); /* push f */
        lua_rawgeti(L, 1, i); /* push t[i] */
        lua_call(L, 1, 1); /* call f(t[i]) */
        lua_rawseti(L, 1, i); /* t[i] = result */
    }
    return 0; /* no results */
}

}

BOOST_AUTO_TEST_CASE( TestArrayManipulation )
{  
    using namespace ProgrammingLua;

    {
        FILE *fp = fopen( "test.lua", "wt" );
        fprintf( fp, "function f ( x ) \n" );
        fprintf( fp, "  return x^2 \n" );
        fprintf( fp, "end \n" );
        fprintf( fp, "t = { 1, 2, 3 } \n" );
        fprintf( fp, "mymap( t, f ) \n" );
        fclose( fp );
    }

    lua_State *L = luaL_newstate();

    luaL_openlibs( L );

    lua_pushcfunction( L, l_map );
    lua_setglobal( L, "mymap" );

    if( luaL_loadfile( L, "test.lua" ) || 
        lua_pcall( L, 0, 0, 0 ) )
    {
        error( L, "cannot run config file : %s", lua_tostring(L,-1) );
    }

    {
        lua_getglobal( L, "t" );

        lua_rawgeti(L,1,1);
        BOOST_CHECK_EQUAL( 1, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        lua_rawgeti(L,1,2);
        BOOST_CHECK_EQUAL( 4, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        lua_rawgeti(L,1,3);
        BOOST_CHECK_EQUAL( 9, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        lua_pop( L, 1 );
    }

    lua_close( L );
}


namespace ProgrammingLua
{

static int l_split (lua_State *L) 
{
    const char *s = luaL_checkstring(L, 1);
    const char *sep = luaL_checkstring(L, 2);
    const char *e;
    int i = 1;
    lua_newtable(L); /* result */
    /* repeat for each separator */
    while ((e = strchr(s, *sep)) != NULL) {
        lua_pushlstring(L, s, e-s); /* push substring */
        lua_rawseti(L, -2, i++);
        s = e + 1; /* skip separator */
    }
    /* push last substring */
    lua_pushstring(L, s);
    lua_rawseti(L, -2, i);
    return 1; /* return the table */
}

}

BOOST_AUTO_TEST_CASE( TestStringManipulation_Split )
{  
    using namespace ProgrammingLua;

    {
        FILE *fp = fopen( "test.lua", "wt" );
        fprintf( fp, "t = mysplit( \"hi,ho,there\", \",\" )" );
        fclose( fp );
    }

    lua_State *L = luaL_newstate();

    luaL_openlibs( L );

    lua_pushcfunction( L, l_split );
    lua_setglobal( L, "mysplit" );

    if( luaL_loadfile( L, "test.lua" ) || 
        lua_pcall( L, 0, 0, 0 ) )
    {
        error( L, "cannot run config file : %s", lua_tostring(L,-1) );
    }

    {
        lua_getglobal( L, "t" );

        lua_rawgeti(L,1,1);
        BOOST_CHECK_EQUAL( "hi", luaL_checkstring( L, -1 ) );
        lua_pop( L, 1 );

        lua_rawgeti(L,1,2);
        BOOST_CHECK_EQUAL( "ho", luaL_checkstring( L, -1 ) );
        lua_pop( L, 1 );

        lua_rawgeti(L,1,3);
        BOOST_CHECK_EQUAL( "there", luaL_checkstring( L, -1 ) );
        lua_pop( L, 1 );

        lua_pop( L, 1 );
    }

    lua_close( L );
}

static const char Key = 'k';

BOOST_AUTO_TEST_CASE( TestRegistry)
{  
    using namespace ProgrammingLua;

    lua_State *L = luaL_newstate();
    luaL_openlibs( L );

    {
        lua_pushnumber( L, 123 );
        lua_setfield( L, LUA_REGISTRYINDEX, "Key.1" );

        lua_getfield( L, LUA_REGISTRYINDEX, "Key.1" );
        BOOST_CHECK_EQUAL( 123, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );
    }

    {
        int r = luaL_ref(L, LUA_REGISTRYINDEX);

        lua_pushnumber( L, 345 );
        lua_rawseti(L, LUA_REGISTRYINDEX, r);

        lua_rawgeti(L, LUA_REGISTRYINDEX, r);
        BOOST_CHECK_EQUAL( 345, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        luaL_unref(L, LUA_REGISTRYINDEX, r);
    }

    {
        /* store a string */
        lua_pushlightuserdata(L, (void *)&Key); /* push address */
        lua_pushstring(L, "Hello"); /* push value */
        lua_settable(L, LUA_REGISTRYINDEX); /* registry[&Key] = myStr */

        /* retrieve a string */
        lua_pushlightuserdata(L, (void *)&Key); /* push address */
        lua_gettable(L, LUA_REGISTRYINDEX); /* retrieve value */
        BOOST_CHECK_EQUAL( "Hello", lua_tostring(L, -1) );
    }

    lua_close( L );
}


namespace ProgrammingLua
{

static int counter (lua_State *L) 
{
    int val = lua_tointeger(L, lua_upvalueindex(1));
    lua_pushinteger(L, ++val); /* new value */
    lua_pushvalue(L, -1); /* duplicate it */
    lua_replace(L, lua_upvalueindex(1)); /* update upvalue */
    return 1; /* return new value */
}

static int newCounter (lua_State *L) 
{
    lua_pushinteger(L, 0);
    lua_pushcclosure(L, &counter, 1);
    return 1;
}

}

BOOST_AUTO_TEST_CASE( TestUpValue)
{  
    using namespace ProgrammingLua;
    
    {
        FILE *fp = fopen( "test.lua", "wt" );
        fprintf( fp, "counterFn = myCounter()" );
        fprintf( fp, "c1 = counterFn()" );
        fprintf( fp, "c2 = counterFn()" );
        fclose( fp );
    }

    lua_State *L = luaL_newstate();
    luaL_openlibs( L );

    lua_pushcfunction( L, newCounter );
    lua_setglobal( L, "myCounter" );

    if( luaL_loadfile( L, "test.lua" ) || 
        lua_pcall( L, 0, 0, 0 ) )
    {
        error( L, "cannot run config file : %s", lua_tostring(L,-1) );
    }

    {
        lua_getglobal( L, "c1" );
        BOOST_CHECK_EQUAL( 1, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        lua_getglobal( L, "c2" );
        BOOST_CHECK_EQUAL( 2, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );
    }

    lua_close( L );
}

namespace ProgrammingLua
{

int t_tuple (lua_State *L) {
    int op = luaL_optint(L, 1, 0);
    if (op == 0) { /* no arguments? */
        int i;
        /* push each valid upvalue onto the stack */
        for (i = 1; !lua_isnone(L, lua_upvalueindex(i)); i++)
            lua_pushvalue(L, lua_upvalueindex(i));
        return i - 1; /* number of values in the stack */
    }
    else { /* get field 'op' */
        luaL_argcheck(L, 0 < op, 1, "index out of range");
        if (lua_isnone(L, lua_upvalueindex(op)))
            return 0; /* no such field */
        lua_pushvalue(L, lua_upvalueindex(op));
        return 1;
    }
}
int t_new (lua_State *L) {
    lua_pushcclosure(L, t_tuple, lua_gettop(L));
    return 1;
}
static const struct luaL_Reg tuplelib [] = {
    {"new", t_new},
    {NULL, NULL}
};

}

BOOST_AUTO_TEST_CASE( TestUpValue_Tuple)
{
    using namespace ProgrammingLua;
    
    {
        FILE *fp = fopen( "test.lua", "wt" );
        fprintf( fp, "x = tuple.new( 10, \"hi\", { }, 3 )" );
        fprintf( fp, "t1 = x(1)" );
        fprintf( fp, "t2 = x(2)" );
        fprintf( fp, "t3 = x(3)" );
        fprintf( fp, "t4 = x(4)" );
        fprintf( fp, "t = x()" );
        fclose( fp );
    }

    lua_State *L = luaL_newstate();
    luaL_openlibs( L );

    luaL_register(L, "tuple", tuplelib);

    if( luaL_loadfile( L, "test.lua" ) || lua_pcall( L, 0, 0, 0 ) )
    {
        error( L, "cannot run config file : %s", lua_tostring(L,-1) );
    }

    {
        lua_getglobal( L, "t1" );
        BOOST_CHECK_EQUAL( 10, lua_tonumber( L, -1 ) );
        lua_pop( L, 1 );

        lua_getglobal( L, "t2" );
        BOOST_CHECK_EQUAL( "hi", luaL_checkstring( L, -1 ) );
        lua_pop( L, 1 );
    }

    lua_close( L );
}

