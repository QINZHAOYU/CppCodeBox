/** *****************************************************************************
*   @copyright :  Copyright (C) 2022 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @see       :  https://github.com/QINZHAOYU
*   @brief     :  To implement .
*
*   Change History:
*   -----------------------------------------------------------------------------
*   v1.0, 2022/01/09, Qin ZhaoYu, zhaoyu.qin@foxmail.com
*   Init model.
*
** ******************************************************************************/
#pragma once
#include "common/CommHeader.hpp"
#include <type_traits>
#include <functional>


namespace ccb
{
template<typename T>
class Task;

template<typename R, typename... Args>
class Task<R(Args...)>
{
public:
	Task(std::function<R(Args...)> &&f) : _fn(std::move(f))
	{}
	Task(std::function<R(Args...)> &f) : _fn(f)
	{}

	R Run(Args &&... args)
	{
		return _fn(std::forward<Args>(args)...);
	}

	template<typename F>
	auto Then(F &&f) -> Task<typename std::result_of<F(R)>::type (Args...)>
	{
		// get the return type of F.
		using return_t = typename std::result_of<F(R)>::type;

		auto func = std::move(_fn);
		return Task<return_t(Args...)> ([func, f](Args && ... args)
		{
			// use the output if previous function as the input of current function.
			return f(func(std::forward<Args>(args)...));
		});
	}

private:
	std::function<R(Args...)> _fn;
};


}

