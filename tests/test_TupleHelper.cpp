#include "tools/Catch/catch.hpp"
#include "modules/TupleHelper.hpp"

using namespace ccb;


TEST_CASE("tests of class TuplePrinter")
{
    SECTION("tests")
    {
        tuple<int, short, double, char> tp = std::make_tuple(1, 3, 5, 'a');
        PrintTuple(tp);
    }
}


TEST_CASE("tests of class findInex")
{
    SECTION("tests")
    {
        tuple<int, double, string> tp = std::make_tuple(1, 12.2, "hello");
        int index = findIndex(tp, "hello");
        CHECK(index == 2);
    }
}

