/** *****************************************************************************
*   @copyright :  Copyright (C) 2022 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @brief     :  To implement some external tuple helper class.
*
*   because the `k` is not appeared in input arguments, therefore,
*   there is no automatic derivation of its type. While `Tuple` does.
*  
*   template can be instantiated (partial) explicitly and implicitly.
*   explicit instantiation can improve efficiency. it used as
*   `tempalte void func<T>(T)`; It is also important to distinguish
*   between explicit instantiation and explicit materialization.
*  
*   template can be specialized by templates.
*   template can be overloaded.
*  
*   template parameters can be non-type(built-on types, limited to
*   + int/char/long/unsigned/bool/short/size_t, 
*   + pointers and lvalue references to objects)
*   which must be compile-time constants, a constant of a fixed type.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   --version-1.0, 2022/01/12, Qin ZhaoYu,
*   To init model.
*
** ******************************************************************************/
#pragma once
#include "common/CommHeader.hpp"
#include <type_traits>


namespace ccb
{
/// \brief To print all element of a tuple.
template<class Tuple, std::size_t N>
struct TuplePrinter
{
    static void print(const Tuple &t)
    {
        TuplePrinter<Tuple, N-1>::print(t);
        cout << ", " << std::get<N-1>(t);
    }
};

template<class Tuple>
struct TuplePrinter<Tuple, 1>
{
    static void print(const Tuple &t)
    {
        cout << std::get<0>(t);
    }
};

template<class... Args>
void PrintTuple(const tuple<Args...> &t)
{
    cout << "(";
    TuplePrinter<decltype(t), sizeof...(Args)>::print(t);
    cout << ")\n";
}


/// \brief Namespace to contains tuple function details.
namespace detail
{
/// \brief To directly compare values of convertible types.
template<typename T, typename U>
typename std::enable_if<std::is_convertible<T, U>::value || std::is_convertible<U, T>::value, bool>::type 
compare(T t, U u)
{
    return t == u;
}


/// \brief To return false if un-convertible.
bool compare(...)
{
    return false;
}


/// \brief To find index with given value.
/// Note that this function would return index of first matched element.
template<int I, typename T, typename... Args>
struct findIndex
{
    static int call(const std::tuple<Args...> &t, T &&val)
    {
        return (compare(std::get<I-1>(t), val) 
        ? I - 1
        : findIndex<I - 1, T, Args...>::call(t, std::forward<T>(val)));
    }
};

template<typename T, typename... Args>
struct findIndex<0, T, Args...>
{
    static int call(const std::tuple<Args...> &t, T &&val)
    {
        return compare(std::get<0>(t), val) ? 0 : -1;
    }
};

}

template<typename T, typename... Args>
int findIndex(const std::tuple<Args...> &t, T &&val)
{
    return detail::findIndex<sizeof...(Args), T, Args...>::call(t, std::forward<T>(val));
}


/// \brief To get value at given index in runtime.
template<size_t k, typename Tuple>
typename std::enable_if<(k == std::tuple_size<Tuple>::value)>::type
GetArgByIndex(size_t index, Tuple &tp)
{
    throw std::invalid_argument("arg index out of range.");
}

template<size_t k=0, typename Tuple>
typename std::enable_if<(k < std::tuple_size<Tuple>::value)>::type
GetArgByIndex(size_t index, Tuple &tp)
{
    if (k == index)
    {
        cout << std::get<k>(tp) << endl;
    }
    else
    {
        GetArgByIndex<k+1>(index, tp); // k++.
    }
}


/// \brief To apply tuple to funciton as its parameters.
template<int...>
struct IndexTuple
{};

template<int N, int... Indexes>
struct MakeIndexes : MakeIndexes<N-1, N-1, Indexes...>
{};

template<int... indexes>
struct MakeIndexes<0, indexes...>
{
    using type = typename IndexTuple<indexes...>;
};

template<typename F, typename Tuple, int... Indexes>
auto apply(F&& f, IndexTuple<Indexes...>&& in, Tuple&& tp) 
-> decltype(std::forward<F>(f)(std::get<Indexes>(tp)...))
{
    // tuple tp expanded as arguments of function f.
    return std::forward<F>(f)(std::get<Indexes>(tp)...); 
}


}

