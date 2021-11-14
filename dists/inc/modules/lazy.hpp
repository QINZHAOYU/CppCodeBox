/** *****************************************************************************
*   @copyright :  Copyright (C) 2021 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @brief     :  To implement class Lazy based on classOptional, which is similar
*                 to Lazy<T> in .NET4.0 .
*
*   Change History:
*   -----------------------------------------------------------------------------
*   --version-1.0, 2021/10/09, Qin ZhaoYu,
*   To init model.
*
** ******************************************************************************/
#pragma once

#include "optional.hpp"
#include <functional>


template<typename T>
class Lazy
{
public:
	Lazy() {}

	template<typename Func, typename... Args>
	Lazy(Func &f, Args... args) // function and arguments have binded for using later.
	{
		//_func = [&f, &args...]() {return f(args...);};
		_func = bind(f, forward<Args>(args)...);  // _func has no arguments, bug f has.
	}

	T &value()
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
	std::function<T()> _func; // one function without arguments.
	Optional<T> _value;
};


// auxiliary function to simplify calling of class Lazy.
template<class Func, typename... Args>
Lazy<typename std::result_of<Func(Args...)>::type> lazy(Func &&func, Args &&... args)
{
	return Lazy<typename result_of<Func(Args...)>::type>(forward<Func>(func), forward<Args>(args)...);
}