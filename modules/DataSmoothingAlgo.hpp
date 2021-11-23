/** *****************************************************************************
*   @copyright :  Copyright (C) 2021 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @see       :  https://github.com/QINZHAOYU
*   @brief     :  To smooth data series.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   v1.0, 2021/11/23, Qin ZhaoYu, zhaoyu.qin@foxmail.com
*   Init model.
*
** ******************************************************************************/

#pragma once
#include "common/CommHeader.hpp"
#include "common/CommStructs.hpp"


/// \brief namespace of cpp code box.
namespace ccb
{

/// \brief To smooth data series.
class DataSmoother
{
public:
	static void linearSmoothN3(const VecDbl &orig, VecDbl &res);
	static void linearSmoothN5(const VecDbl &orig, VecDbl &res);
	static void linearSmoothN7(const VecDbl &orig, VecDbl &res);

	static void quadraticSmoothN5(const VecDbl &orig, VecDbl &res);
	static void quadraticSmoothN7(const VecDbl &orig, VecDbl &res);

	static void cubicSmoothN5(const VecDbl &orig, VecDbl &res);
	static void cubicSmoothN7(const VecDbl &orig, VecDbl &res);

private:
	static double smoother(const VecDbl &elems, const VecDbl &args);
	static double smoother(const VecDbl &elems);
	static VecDbl reverseVec(VecDbl &&elems);
};

}  // end of namespace ccb.