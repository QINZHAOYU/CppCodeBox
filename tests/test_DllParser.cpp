#include "tools/Catch/catch.hpp"
#include "modules/DllParser.hpp"

using namespace ccb;


TEST_CASE("tests for class WinDllParser")
{
    DllParser dllParser;

#ifdef WINDOWS
    SECTION("test of functions with args on windows")
	{
	    string str("data_DllParser.dll");
    
	    CHECK(dllParser.Load(str));
    
	    int a = 5;
	    int b = 8;
    
	    auto max = dllParser.ExcecuteFunc<int(const int&, const int&)>("Max", a, b);
        CHECK(max == 8);
	    auto add = dllParser.ExcecuteFunc<int(int&, int&)>("Add", a, b);
        CHECK(add == 13);

        CHECK(dllParser.UnLoad());
        REQUIRE_THROWS_AS(dllParser.ExcecuteFunc<int(int&, int&)>("Add", a, b), std::exception);
    }

    SECTION("test of function without args on windows")
    {
        string str("MyCppKits.dll");
        CHECK(dllParser.Load(str));

        REQUIRE(dllParser.GetFunction<void()>("printHello") != nullptr);
        dllParser.ExcecuteFunc<void()>("printHello");
    }

#elif LINUX
    SECTION("test of function without args on linux")
    {
        string str("MyCppKits.so");
        CHECK(dllParser.Load(str));

        REQUIRE(dllParser.GetFunction<void()>("printHello") != nullptr);
        dllParser.ExcecuteFunc<void()>("printHello");        
    }

#endif 
    dllParser.~DllParser();
}





