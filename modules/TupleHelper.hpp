/** *****************************************************************************
*   @copyright :  Copyright (C) 2022 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @brief     :  To implement some external tuple helper class.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   --version-1.0, 2022/01/12, Qin ZhaoYu,
*   To init model.
*
** ******************************************************************************/
#pragma once
#include "common/CommHeader.hpp"


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


/// \brief Namespace to contains index searching details.
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


}

