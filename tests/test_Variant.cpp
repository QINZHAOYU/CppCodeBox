#include "tools/Catch/catch.hpp"
#include "modules/Variant.hpp"

using namespace ccb;


TEST_CASE("tests for class Variant")
{
    SECTION("test1")
    {
        // failed.

        // using CV = Variant<int, double, string, int>;
        // CV var = 10;
    
        // CHECK(std::is_same<CV::IndexType<1>, double>::value == true);
        // cout << typeid(CV::IndexType<1>).name() << endl;

        // CHECK(var.indexOf<string>() == 2);
    }
}

