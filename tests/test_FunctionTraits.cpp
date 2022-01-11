#include "modules/FunctionTraits.hpp"
#include "tools/Catch/catch.hpp"

using namespace ccb;


template<typename T>
void PrintType()
{
    cout << typeid(T).name() << endl;
}

float (*castFunc)(string, int);

float free_function(const string &a, int b)
{
    return (float) a.size() / b;
}

class AA
{
public:
    int func(int a, int b) volatile
    {
        return a + b;
    }

    int operator()(int)
    {
        return 0;
    }
};


TEST_CASE("tests of class function_traits")
{
    SECTION("all tests")
    {
        std::function<int(int)> f = [](int a) { return a; };

        // std::function.
        using funct_type = function_traits<std::function<int(int)>>::function_type;
        CHECK(std::is_same<funct_type, int(int)>::value == true);
        CHECK(std::is_same<funct_type, std::function<int(int)>>::value == false);        
        PrintType<funct_type>();

        using arg_type = function_traits<std::function<int(int)>>::args<0>::type;
        using arg_type_2 = function_traits<std::function<int(int)>>::arg_type<0>;
        CHECK(std::is_same<arg_type, int>::value == true);
        CHECK(std::is_same<arg_type, arg_type_2>::value == true);
        PrintType<arg_type>();

        using funct_type_2 = function_traits<decltype(f)>::function_type;
        CHECK(std::is_same<funct_type, funct_type_2>::value == true);
        CHECK(std::is_same<funct_type, decltype(f)>::value == false); 
        PrintType<funct_type_2>(); // int __cdecl(int);
        PrintType<decltype(f)>();  // std::function<int(int)>

        static_assert(std::is_same<function_traits<decltype(f)>::return_type, int>::value == true);

        // normal function.
        using free_funct_type = function_traits<decltype(free_function)>::function_type;
        CHECK(std::is_same<free_funct_type, float(const string&, int)>::value == true);
        CHECK(std::is_same<free_funct_type, float(const string, int)>::value == false);
        CHECK(std::is_same<free_funct_type, float(string&, int)>::value == false);                
        PrintType<free_funct_type>();

        // function pointer.
        using ptr_funct_type = function_traits<decltype(castFunc)>::function_type;  // function pointer is removed, get bare function type.
        CHECK(std::is_same<ptr_funct_type, float (*)(string, int)>::value == false);  
        PrintType<ptr_funct_type>();  
        PrintType<decltype(castFunc)>(); 

        // class member function.
        using class_member_type = decltype(&AA::func);
        CHECK(std::is_same<class_member_type, int(AA::*)(int, int)>::value == false);
        CHECK(std::is_same<class_member_type, int(AA::*)(int, int)volatile>::value == true);
        PrintType<class_member_type>();

        using class_member_type_2 = function_traits<decltype(&AA::func)>::function_type;
        CHECK(std::is_same<class_member_type, class_member_type_2>::value == false);
        CHECK(std::is_same<class_member_type_2, int(int, int)>::value == true);
        PrintType<class_member_type_2>();
    }
}

