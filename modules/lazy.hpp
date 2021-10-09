#pragma once

#include "optional.hpp"
#include <functional>

template<typename T>
class Lazy
{
public:
    Lazy(){}

    template<typename Func, typename... Args>
    Lazy(Func &f, Args... args) // function and arguments have binded for using later.
    {
        //_func = [&f, &args...]() {return f(args...);};
        _func = bind(f, forward<Args>(args)...);  // _func has no arguments, bug f has.
    }

    T& value()
    {
        if (!_value.isInit())
        {
            _value = _func(); // load action now.
        }

        return *_value;
    }

    bool isValueCreated() const
    {
        return _value.isInit();
    }

private:
    function<T()> _func; // one function without arguments.
    Optional<T> _value;
};


// auxiliary function to simplify calling of class Lazy.
template<class Func, typename... Args>
Lazy<typename result_of<Func(Args...)>::type> lazy(Func&& func, Args&&... args)
{
    return Lazy<typename result_of<Func(Args...)>::type>(forward<Func>(func), forward<Args>(args)...);
}