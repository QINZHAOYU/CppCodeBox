#include "tools/Catch/catch.hpp"
#include "modules/TaskList.hpp"

using namespace ccb;


TEST_CASE("tests of class TaskList")
{
    SECTION("test of listing reducing")
    {
        Task<int(int)> task([](int i){ return i; });

        auto result = task
        .Then([](int i){ return i + 1; })
        .Then([](int i){ return i + 2; })
        .Then([](int i){ return i + 3; })
        .Run(1);

        REQUIRE(result == 7);
    }
}

