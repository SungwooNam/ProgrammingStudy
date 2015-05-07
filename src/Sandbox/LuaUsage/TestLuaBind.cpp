#include <boost/test/unit_test.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>
#include <fstream>

// need preprocessor : LUABIND_DYNAMIC_LINK
#pragma warning(disable:4251)
#include "luabind\luabind.hpp"
#include "luabind\object.hpp"

#include "LuaCommon.h"

using namespace std;
using namespace boost;
using namespace luabind;

namespace TestLuaBind
{

void greet() { std::cout << "hello world!\n"; }

BOOST_AUTO_TEST_CASE( TestHelloWorld )
{  
    {
        ofstream to( "test.lua", ios_base::out );
        to <<  
            "greet()"
        ;
    }

    lua_State *L = luaL_newstate();
    luaL_openlibs( L );

    {
        open(L);
        module(L)
        [
            def("greet", &greet)
        ];
    }

    try 
    {
        if( luaL_loadfile( L, "test.lua" ) || lua_pcall( L, 0, 0, 0 ) )
        {
            throw runtime_error( lua_tostring(L, -1) );
        }
    }
    catch( runtime_error& ex )
    {
        printf( ex.what() );
    }

    lua_close( L );
}


void SayHello() { std::cout << "Hello There\n"; }
void SayHello( const std::string &to ) { std::cout << "Hello " << to << "\n"; }

BOOST_AUTO_TEST_CASE( TestScope )
{  
    {
        ofstream to( "test.lua", ios_base::out );
        to << 
            "MyModule.SayHello()  "
            "MyModule.SayHello( \"Kiung\" )  "
        ;
    }

    lua_State *L = luaL_newstate();

    {
        open(L);
        module(L, "MyModule" )
        [
            def( "SayHello", (void(*)()) &SayHello),
            def( "SayHello", (void(*)(const string&)) &SayHello)
        ];
    }

    try 
    {
        if( luaL_loadfile( L, "test.lua" ) || lua_pcall( L, 0, 0, 0 ) )
        {
            throw runtime_error( lua_tostring(L, -1) );
        }
    }
    catch( runtime_error& ex )
    {
        printf( ex.what() );
    }

    lua_close( L );
}


BOOST_AUTO_TEST_CASE( TestCallingLuaFunction )
{  
    {
        ofstream to( "test.lua", ios_base::out );
        to << 
            "function MyPower( x )  "
            "   return x^2          "
            "end                    "
        ;
    }

    lua_State *L = luaL_newstate();

    try 
    {
        if( luaL_loadfile( L, "test.lua" ) || lua_pcall( L, 0, 0, 0 ) )
        {
            throw runtime_error( lua_tostring(L, -1) );
        }

        int ret = call_function<int>( L, "MyPower", 2 );
        BOOST_CHECK_EQUAL( 4, ret );

    }
    catch( runtime_error& ex )
    {
        printf( ex.what() );
    }

    lua_close( L );
}


class testclass
{
public:
    testclass(const std::string& s): m_string(s) {}
    int print_string() { std::cout << m_string << "\n"; return 10; }
    int Test() { std::cout << m_string << "\n"; return 20; }
    int Test( const std::string& arg ) { std::cout << m_string << arg << "\n"; return 30; }

private:
    std::string m_string;
};

BOOST_AUTO_TEST_CASE( TestBindingClassToLua )
{  
    {
        ofstream to( "test.lua", ios_base::out );
        to << 
            "a=testclass('a string')      "
            "b=a:print_string()           "
            "c=a:Test()                   "
            "d=a:Test('test')             "
        ;
    }

    lua_State *L = luaL_newstate();

    try 
    {
        open(L);
        module(L)
        [
            class_<testclass>("testclass")
                .def(constructor<const std::string&>())
                .def("print_string", &testclass::print_string)
                .def("Test", (int(testclass::*)())&testclass::Test)
                .def("Test", (int(testclass::*)(const std::string&))&testclass::Test)
        ];

        if( luaL_loadfile( L, "test.lua" ) || lua_pcall( L, 0, 0, 0 ) )
        {
            throw runtime_error( lua_tostring(L, -1) );
        }
         
        object G( globals(L) );
        int b = object_cast<int>(G["b"]);
        BOOST_CHECK_EQUAL( 10, b );

        BOOST_CHECK_EQUAL( 20, object_cast<int>(G["c"]) );
        BOOST_CHECK_EQUAL( 30, object_cast<int>(G["d"]) );
    }

    catch( runtime_error& ex )
    {
        printf( ex.what() );
    }

    lua_close( L );
}


class Baby
{
public:
    virtual ~Baby() {}

    int Age;

    enum Activity
    {
        Sleep = 0,
        Eat = 1,
        WaWa = 2,
    };

    Activity get_doing() const { return m_do; }
    void set_doing( Activity doing ) { m_do = doing; }

    virtual std::string Mother() { return "aaa"; }

private:
    Activity m_do;
};


BOOST_AUTO_TEST_CASE( TestBindingPropertiesAndEnum )
{  
    {
        ofstream to( "test.lua", ios_base::out );
        to << 
            "leeanne=Baby()               "
            "leeanne.Age=1                "
            "a = leeanne.Age              "
            "leeanne.Activity = Baby.WaWa "
            "b = leeanne.Activity         "
        ;
    }

    lua_State *L = luaL_newstate();

    try 
    {
        open(L);
        module(L)
        [
            class_<Baby>("Baby")
                .def(constructor<>())
                .def_readwrite("Age", &Baby::Age)
                .property( "Activity", &Baby::get_doing, &Baby::set_doing )
                .enum_( "constants" )
                [
                    value("Sleep", 0),
                    value("Eat",1),
                    value("WaWa",2)
                ]
        ];

        if( luaL_loadfile( L, "test.lua" ) || lua_pcall( L, 0, 0, 0 ) )
        {
            throw runtime_error( lua_tostring(L, -1) );
        }
         
        object G( globals(L) );
        int a = object_cast<int>(G["a"]);
        BOOST_CHECK_EQUAL( 1, a );

        Baby::Activity b = object_cast<Baby::Activity>(G["b"]);
        BOOST_CHECK_EQUAL( Baby::WaWa, b );
    }

    catch( runtime_error& ex )
    {
        printf( ex.what() );
    }

    lua_close( L );
}



std::string CommonBabling()
{
    return std::string( "Dadada" );
}

BOOST_AUTO_TEST_CASE( TestClassScope )
{  
    {
        ofstream to( "test.lua", ios_base::out );
        to << 
            "a=Baby.CommonBabling()       "
        ;
    }

    lua_State *L = luaL_newstate();

    try 
    {
        open(L);
        module(L)
        [
            class_<Baby>("Baby")
                .def(constructor<>())
                .scope
                [
                    def( "CommonBabling", &CommonBabling )
                ]
        ];

        if( luaL_loadfile( L, "test.lua" ) || lua_pcall( L, 0, 0, 0 ) )
        {
            throw runtime_error( lua_tostring(L, -1) );
        }
         
        object G( globals(L) );
        std::string a = object_cast<std::string>(G["a"]);
        BOOST_CHECK_EQUAL( "Dadada", a.c_str() );
    }

    catch( runtime_error& ex )
    {
        printf( ex.what() );
    }

    lua_close( L );
}



class IBaby 
{
public:
    IBaby() { _RefCount++; }
    virtual ~IBaby() { _RefCount--; };
    virtual std::string CallMother() = 0;

protected:
    static int _RefCount;
public:
    static int RefCount() { return _RefCount; }
};

int IBaby::_RefCount = 0;

class KoreanBaby : public IBaby
{
public:
    std::string CallMother() { return std::string("Umma"); }
};

class EnglishBaby : public IBaby
{
public:
    std::string CallMother() { return std::string("Mama"); }
};


BOOST_AUTO_TEST_CASE( TestDerivedClass )
{  
    {
        ofstream to( "test.lua", ios_base::out );
        to << 
            "leeanne=KoreanBaby()       "
            "a=leeanne:CallMother()     "
            "laura=EnglishBaby()        "
            "b=laura:CallMother()           "
        ;
    }

    lua_State *L = luaL_newstate();

    try 
    {
        open(L);
        module(L)
        [
            class_<IBaby>("IBaby")
                .def( "CallMother", &IBaby::CallMother )
                ,

            class_<KoreanBaby,IBaby>("KoreanBaby")
                .def(constructor<>())
                ,

            class_<EnglishBaby,IBaby>("EnglishBaby")
                .def(constructor<>())
        ];

        if( luaL_loadfile( L, "test.lua" ) || lua_pcall( L, 0, 0, 0 ) )
        {
            throw runtime_error( lua_tostring(L, -1) );
        }
         
        object G( globals(L) );
        std::string a = object_cast<std::string>(G["a"]);
        BOOST_CHECK_EQUAL( "Umma", a.c_str() );
        
        std::string b = object_cast<std::string>(G["b"]);
        BOOST_CHECK_EQUAL( "Mama", b.c_str() );
    }

    catch( runtime_error& ex )
    {
        printf( ex.what() );
    }

    lua_close( L );
}


BOOST_AUTO_TEST_CASE( TestPreRegisterClassInstance )
{  
    {
        ofstream to( "test.lua", ios_base::out );
        to << 
            "a = Leeanne:CallMother()           "
            "b = Laura:CallMother()           "
        ;
    }

    lua_State *L = luaL_newstate();

    BOOST_CHECK_EQUAL( 0, IBaby::RefCount() );
    boost::shared_ptr<IBaby> Leeanne( new KoreanBaby() );
    boost::shared_ptr<IBaby> Laura( new EnglishBaby() );
    BOOST_CHECK_EQUAL( 2, IBaby::RefCount() );

    try 
    {
        open(L);
        module(L)
        [
            class_<IBaby, boost::shared_ptr<IBaby> >("IBaby")
                .def( "CallMother", &IBaby::CallMother )
        ];

        {
            object G( globals(L) );

            G["Leeanne"] = Leeanne;
            G["Laura"] = Laura;
        }

        if( luaL_loadfile( L, "test.lua" ) || lua_pcall( L, 0, 0, 0 ) )
        {
            throw runtime_error( lua_tostring(L, -1) );
        }
         
        object G( globals(L) );
        std::string a = object_cast<std::string>(G["a"]);
        BOOST_CHECK_EQUAL( "Umma", a.c_str() );
        
        std::string b = object_cast<std::string>(G["b"]);
        BOOST_CHECK_EQUAL( "Mama", b.c_str() );
    }

    catch( runtime_error& ex )
    {
        printf( ex.what() );
    }

    lua_close( L );


    BOOST_CHECK_EQUAL( 2, IBaby::RefCount() );
    Leeanne.reset();
    Laura.reset();
    BOOST_CHECK_EQUAL( 0, IBaby::RefCount() );
}


class Engine 
{
public:
    virtual ~Engine() {}
    void Enqueue( luabind::object argFn, luabind::object argParam )
    {
        m_Fn = argFn;
        m_Param = argParam;
    }

    void Run()
    {
        m_Fn( m_Param );
    }

private:
    luabind::object m_Fn;
    luabind::object m_Param;
};


BOOST_AUTO_TEST_CASE( TestLuaFunctionCallWithArgument )
{  
    {
        ofstream to( "test.lua", ios_base::out );
        to << 
            "param = { sequence = \"\" }                \n"
            "function seq1( p )                         \n"
            "   p.sequence = p.sequence .. \"1\"        \n"
            "end                                        \n"
            "ENGINE:Enqueue( seq1, param )              \n"
        ;
    }

    lua_State *L = luaL_newstate();

    boost::shared_ptr<Engine> engine( new Engine() );

    try 
    {
        open(L);
        module(L)
        [
            class_<Engine, boost::shared_ptr<Engine> >("Engine")
                .def( "Enqueue", &Engine::Enqueue )
        ];

        {
            object G( globals(L) );
            G["ENGINE"] = engine;
        }

        if( luaL_loadfile( L, "test.lua" ) || lua_pcall( L, 0, 0, 0 ) )
        {
            throw runtime_error( lua_tostring(L, -1) );
        }

        engine->Run();
        engine->Run();

        {
            object G( globals(L) );
            std::string seq = object_cast<std::string>(G["param"]["sequence"]);
            BOOST_CHECK_EQUAL( "11", seq.c_str() );
        }
    }

    catch( runtime_error& ex )
    {
        printf( ex.what() );
    }

    engine.reset();

    lua_close( L );
}

}