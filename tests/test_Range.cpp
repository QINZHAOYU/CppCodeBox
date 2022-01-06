#include "modules/range.hpp"
#include "tools/Catch/catch.hpp"
#include "common/Commstructs.hpp"
#include <numeric>

using namespace ccb;


TEST_CASE("range tests")
{
	SECTION("range::range(T end)")
	{
		VecInt comp(15);
		std::iota(comp.begin(), comp.end(), 0);

		VecInt res;
		cout << "\nrange(15):";
		for (int i : Range(15))  //[0,15)
		{
			cout << " " << i;
			res.push_back(i);
		}

		REQUIRE(comp.size() == res.size());
		REQUIRE(comp == res);
	}

	SECTION("range::range(T begin, T  end)")
	{
		int size = 0;

		cout << "\nrange(2,6):";
		for (auto i : Range(2, 6))  //[2,6)
		{
			cout << " " << i;
			size ++;
		}
		CHECK(size == 4);

		cout << "\nrange(10.5, 15.5):";
		size = 0;
		for (auto i : Range(10.5, 15.5))  //[10.5,15.5), 步长为1
		{
			cout << " " << i;
			size ++;
		}
		CHECK(size == 5);


		cout << "\nrange('a', 'z'):";
		size = 0;
		for (auto i : Range('a', 'z'))  //['a','z'), 步长为1
		{
			cout << " " << i;
			size ++;
		}
		CHECK(size == 25);
	}

	SECTION("range::range(T begin, T end, U step)")
	{
		int size = 0;

		cout << "\nrange(35,27,-1):";
		for (int i : Range(35, 27, -1))  //(27, 35]，步长为-1
		{
			cout << " " << i;
			size ++;
		}
		CHECK(size == 8);

		cout << "\nrange(2,8,0.5):";
		size = 0;
		for (auto i : Range(2, 8, 0.5))  //[2,8), 步长为0.5
		{
			cout << " " << i;
			size ++;
		}
		CHECK(size == 12);

		cout << "\nrange(8,7,-0.1):";
		size = 0;
		for (auto i : Range(8, 7, -0.1))  //(7,8]，步长为-0.1
		{
			cout << " " << i;
			size ++;
		}
		CHECK(size == 10);
	}

	SECTION("range exception")
	{
		REQUIRE_THROWS_AS(Range(-5), std::logic_error);
		REQUIRE_THROWS_AS(Range(5, 1), std::logic_error);
		REQUIRE_THROWS_AS(Range(1, 5, -1), std::logic_error);
	}

}




