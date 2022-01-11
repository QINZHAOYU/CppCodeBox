/** *****************************************************************************
*   @copyright :  Copyright (C) 2021 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @brief     :  To implement class function traits.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   --version-1.0, 2022/01/10, Qin ZhaoYu,
*   To init model.
*
** ******************************************************************************/
#pragma once
#include "common/CommHeader.hpp"
#include <functional>


namespace ccb
{
template<typename T>
class function_traits;


/// \brief Function traits for normal function.
template<typename Ret, typename... Args>
class function_traits<Ret(Args...)>
{
public:
    enum { arity = sizeof...(Args) };
    using function_type = Ret(Args...);
    using return_type = Ret;
    using stl_function_type = std::function<function_type>;
    using pointer = Ret(*)(Args...);

    template<size_t I>
    struct args
    {
        static_assert(I < arity, "index is out of range");

        // to get the type of I-th element in Args.
        using type = typename std::tuple_element<I, tuple<Args...>>::type;
    };

    // using tuple_type = typename std::decay<Args>...::type;
    template<size_t N>
    using arg_type = typename std::tuple_element<N, tuple<Args...>>::type;
};


/// \brief Function traits for funtion pointer.
template<typename Ret, typename... Args>
class function_traits<Ret(*)(Args...)> : public function_traits<Ret(Args...)>
{};


/// \brief Function traits for std::function object.
template<typename Ret, typename... Args>
class function_traits<std::function<Ret(Args...)>> : public function_traits<Ret(Args...)>
{};


/// \brief Function traits for class member function pointer.
/// `_VA_ARGS` is used to contain cv symbol.
// #define FUNCTION_TRAITS(...)\
// template <typename Ret, typename ClassType, typename... Args>\
// class function_traits<Ret(ClassType::*)(Args...) __VA_ARGS__> : public function_traits<Ret(Args...)>{};\

// FUNCTION_TRAITS()
// FUNCTION_TRAITS(const)
// FUNCTION_TRAITS(volatile)
// FUNCTION_TRAITS(const volatile)

template <typename Ret, typename ClassType, typename... Args>
class function_traits<Ret(ClassType::*)(Args...)> : public function_traits<Ret(Args...)>
{};
template <typename Ret, typename ClassType, typename... Args>
class function_traits<Ret(ClassType::*)(Args...) const> : public function_traits<Ret(Args...)>
{};
template <typename Ret, typename ClassType, typename... Args>
class function_traits<Ret(ClassType::*)(Args...) volatile> : public function_traits<Ret(Args...)>
{};
template <typename Ret, typename ClassType, typename... Args>
class function_traits<Ret(ClassType::*)(Args...) const volatile> : public function_traits<Ret(Args...)>
{};

/// \brief Function traits for function object.
template<typename Callable>
class function_traits : public function_traits<decltype(&callable::operator())>
{};


/// \brief Transform function.
template<typename Function>
typename function_traits<Function>::stl_function_type to_function(const Function &lambda)
{
    using return_t = typename function_traits<Function>::stl_function_type;
    return static_cast<return_t>(lambda);
}
template<typename Function>
typename function_traits<Function>::stl_function_type to_function(Function &&lambda)
{
    using return_t = typename function_traits<Function>::stl_function_type;
    return static_cast<return_t>(std::forward<Function>(lambda));
}


/// \brief Tranform function pointer.
template<typename Function>
typename function_traits<Function>::pointer to_function_pointer(const Function &lambda)
{
    using return_t = typename function_traits<Function>::pointer;
    return static_cast<return_t>(lambda);
}


}

