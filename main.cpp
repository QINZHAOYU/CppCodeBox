#include "CommHeaders.hpp"
#include "modules/optional.hpp"
#include "modules/lazy.hpp"


struct BigObject
{
    BigObject()
    {
        cout << "lazy load big object" << endl;
    }
};


struct MyStruct
{
    MyStruct()
    {
        _obj = lazy([]{return make_shared<BigObject>();});
    }

    void load()
    {
        _obj.value();
    }

    Lazy<shared_ptr<BigObject>> _obj;
};


int Foo(int x)
{
    return x*2;
}


int main(int argc, char* argv[])
{
    try
    {
        
    int y = 4;
    auto lazyer1 = lazy(Foo, y);
    cout << lazyer1.value() << endl;

    Lazy<int> lazyer2 = lazy([]{return 12;});
    cout << lazyer2.value() << endl;

    function<int(int)> f = [](int x) {return x+3;};
    auto lazyer3 = lazy(f, 3);
    cout << lazyer3.value() << endl;

    MyStruct t;
    t.load();

    }
    catch(logic_error& exp)
    {
        cout << exp.what() << endl;
    }
    catch(...)
    {
        cout << "err" << endl;
    }

    return 0;
}

