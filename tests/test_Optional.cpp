#include "tools/Catch/catch.hpp"
#include "modules/optional.hpp"

using namespace ccb;


struct MyStruct
{
	MyStruct(){}
	MyStruct(int a, int b) : m_a(a), m_b(b){}
	int m_a;
	int m_b;
};


TEST_CASE("tests of class Optional")
{
    SECTION("built-in variable")
	{
        Optional<std::string> a("ok");
	    Optional<std::string> b("ok");
	    Optional<std::string> c("aa");
    
        if (a) cout << "a is inited: " << *a << endl;
        if (c) cout << "c is inited: " << *c << endl;        

	    c = a;    
        cout << "assign a to c" << endl;
        if (a) cout << "a is inited: " << *a << endl;
        if (c) cout << "c is inited: " << *c << endl; 
    }

    SECTION("custom object")
	{
        Optional<MyStruct> op;
        cout << "if inited: " << op.isInit() << endl;
    
	    op.emplace(1, 2);
        cout << "if inited: " << op.isInit();
        if (op.isInit()) 
        {
            cout << ",  " << (*op).m_a << "   " << (*op).m_b ;
        }cout << endl;
    
	    MyStruct t; 
	    if (op)  // if inited
	    {
	    	t = *op;
	    }
        cout << t.m_a << ", " << t.m_b << endl;
    
	    op.emplace(3, 4);
	    t = *op;
        cout << t.m_a << ", " << t.m_b << endl;
    }
}

