#include "tools/Catch/catch.hpp"
#include "modules/TupleHelper.hpp"

using namespace ccb;


TEST_CASE("tests of struct TuplePrinter")
{
    SECTION("tests")
    {
        tuple<int, short, double, char> tp = std::make_tuple(1, 3, 5, 'a');
        PrintTuple(tp);
    }
}


TEST_CASE("tests of struct findInex")
{
    SECTION("tests")
    {
        tuple<int, double, string> tp = std::make_tuple(1, 12.2, "hello");
        int index = findIndex(tp, "hello");
        CHECK(index == 2);
    }
}



TEST_CASE("tests of function GetArgByIndex")
{
    SECTION("tests")
    {
        using Tuple = tuple<int, double, string, int>;
        Tuple tp = std::make_tuple(1, 12.2, "hello", 23);

        const size_t len = std::tuple_size<Tuple>::value;

        for (size_t i = 0; i < len; ++i)
        {
            GetArgByIndex<0>(i, tp);
        }

        REQUIRE_THROWS_AS(GetArgByIndex(4, tp), std::invalid_argument);

    }
}


TEST_CASE("tests of function apply")
{
    auto func = [](int a, int b)->int{ return a+b; };

    SECTION("tests")
    {
        auto res = apply(func, MakeIndexes<2>::type(), std::make_tuple(1, 2));
        CHECK(res == 3);
    }
}

