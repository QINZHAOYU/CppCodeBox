#include "modules/FuncWrapper.hpp"
#include "tools/Catch/catch.hpp"
#include "common/CommHeader.hpp"
#include "common/CommStructs.hpp"


using namespace ccb;


void test0() { cout << "void\n"; }
int  test1() { return 1; }
int  test2(int x) { return x; }
string test3(string x1, string x2) { return x1 + x2; }
size_t test4(string x) {return x.size(); }
int    test5(VecInt x) {return x.front(); }


TEST_CASE("tests of class FuncWrapper")
{
    SECTION("wrapper of lambda function")
    {
        auto func = [](){ cout << "void\n"; };

        FuncWrapper(func);  
    }

    SECTION("wrapper of void(void)")
    {
        FuncWrapper(test0);
    }

    SECTION("wrapper of int(void)")
    {
        int res = FuncWrapper(test1);
        REQUIRE(res == 1);
    }

    SECTION("wrapper of int(int)")
    {
        int res = FuncWrapper(test2, 2);
        REQUIRE(res == 2);
    }

    SECTION("wrapper of string(string, string)")
    {
        string res = FuncWrapper(test3, "abd", "opq");
        REQUIRE(res == "abdopq");
    }

    SECTION("wrapper of size_t(string)")
    {
        size_t res = FuncWrapper(test4, "abd");
        REQUIRE(res == 3);
    }

    SECTION("wrapper of int(VecInt)")
    {
        int res = FuncWrapper(test5, VecInt(5, 10));
        REQUIRE(res == 10);
    }
}

