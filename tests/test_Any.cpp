#include "tools/Catch/catch.hpp"
#include "modules/Any.hpp"

using namespace ccb;


TEST_CASE("tests of class Any")
{
    SECTION("")
    {
	    Any n;
        CHECK(n.IsNull() == true);

	    string s1 = "hello";
	    n = s1;
        string str = n.AnyCast<string>();
        CHECK(str == "hello");

	    REQUIRE_THROWS_AS(n.AnyCast<int>(), std::bad_cast);

        n = 10;
        CHECK(n.Is<int>() == true);
        int num = n.AnyCast<int>();
        CHECK(num == 10); 

        Any n2(11.22);
        CHECK(n2.Is<double>() == true);

        Any n3("world");
        CHECK(n3.Is<string>() == false);
        CHECK(n3.Is<char*>() == false); 
        CHECK(n3.Is<const char*>() == true);     
    }
}

