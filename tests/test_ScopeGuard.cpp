#include "tools/Catch/catch.hpp"
#include "modules/ScopeGuard.hpp"

using namespace ccb;


TEST_CASE("tests of class ScopeGuard")
{
    std::function<void()> f = []{ cout << "clean up from unnormal exit." ; };

    SECTION("normal exit")
    {
        auto gd = MakeGuard(f);
        gd.Dismiss();
    }

    SECTION("exception exit")
    {
        auto gd = MakeGuard(f);
        throw std::exception("test exception.");
    }

    SECTION("unnormal exit")
    {
        auto gd = MakeGuard(f);
        return;
    }
}

