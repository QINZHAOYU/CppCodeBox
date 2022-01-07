/** *****************************************************************************
*   @copyright :  Copyright (C) 2022 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @see       :  https://github.com/QINZHAOYU
*   @brief     :  To implement a universal function wrapper template.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   v1.0, 2022/01/06, Qin ZhaoYu, zhaoyu.qin@foxmail.com
*   Init model.
*
** ******************************************************************************/
#pragma once


namespace ccb
{
/// \brief A universal function wrapper template.
template<class Function, class... Args>
inline auto FuncWrapper(Function &&f, Args &&... args) 
-> decltype(f(std::forward<Args>(args)...))
{
	return f(std::forward<Args>(args)...);
}

}

