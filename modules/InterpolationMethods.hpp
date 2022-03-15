/** *****************************************************************************
 *   @copyright :  Copyright (C) 2022 Qin ZhaoYu. All rights reserved.
 *
 *   @author    :  Qin ZhaoYu.
 *   @see       :  https://github.com/QINZHAOYU
 *   @brief     :  一些常用的差值方法.
 *
 *   Change History:
 *   -----------------------------------------------------------------------------
 *   v1.0, 2022/03/15, Qin ZhaoYu,
 *   Init model.
 *
 ** ******************************************************************************/
#include <vector>

namespace ccb
{

/// \brief 牛顿插值法。
/// 如果指定的 `order` 阶插值无效，则算法会自动降阶。目前进支持多项式函数插值。
///
/// \param x 现有散点的 x 值数组
/// \param y 现有散点的 y 值数组
/// \param n 现有散点数组的长度
/// \param tarX 待插值的 x 值
/// \param order 牛顿插值法阶数
/// \return 牛顿插值结果。
/// \todo 增强算法对于诸如三角函数等的支持。
double NewtonInterpolation(double x[], double y[], int n, double tarX, int order);

} // namespace ccb