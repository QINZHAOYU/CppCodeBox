#include "tools/Catch/catch.hpp"
#include "modules/lazy.hpp"
#include "common/CommHeader.hpp"

using namespace ccb;


int Foo(int x)
{
	return x * 2;
}

class BigObject
{
public:
	BigObject() { cout << "lazy load big object" << endl; }
};

class MyStruct
{
public:
	MyStruct()
	{
		_obj = lazy([]() {return std::make_shared<BigObject>(); });
	}

	void load()
	{
		_obj.value();
	}

	Lazy<std::shared_ptr<BigObject>> _obj;
};

TEST_CASE("tests of class Lazy")
{
	SECTION("normal function with args")
	{
		int y = 4;
		auto lazyer1 = lazy(Foo, y);
		cout << "lazyer1.Value()  " << lazyer1.value() << endl;
	}

	SECTION("lambda function with no args")
	{
		Lazy<int> lazyer2 = lazy([] {return 12;});
		cout << "lazyer2.Value()  " << lazyer2.value() << endl;
	}

	SECTION("std::function with args")
	{
		std::function<int(int)> f = [](int x) { return x + 3; };
		auto lazyer3 = lazy(f, 4);
		cout << "lazyer3.Value()  " << lazyer3.value() << endl;
	}

	SECTION("lazy laoding of big objects")
	{
		MyStruct t;
		t.load();
	}
}

