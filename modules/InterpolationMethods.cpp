/** *****************************************************************************
 *    @File      :  InterpolationMethods.cpp
 *    @Brief     :  一些常用的差值方法.
 *
 ** ******************************************************************************/
#include "InterpolationMethods.hpp"
#include <vector>

namespace ccb
{
using std::vector;

double NewtonInterpolation(double x[], double y[], int n, double tarX, int order)
{
    if (n <= 0 || order < 0)
    {
        char msg[512];
        sprintf(msg, "Newton interpolation, invalid array length(%d) or order(%d)\n", n, order);
        throw std::invalid_argument(msg);
    }

    if (n <= order)
    {
        order = n - 1;
    }

    vector<vector<double>> table(n);
    for (int i = 0; i < n; i++)
    {
        table[i].resize(n);
    }
    for (int i = 0; i < n; i++)
    {
        table[0][i] = y[i];
    }
    for (int i = 1; i < n; i++)
    {
        for (int j = i; j < n; j++)
        {
            table[i][j] = (table[i - 1][j] - table[i - 1][j - 1]) / (x[j] - x[j - i]);
        }
    }

    for (int i = order; i >= 0; --i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (std::isnan(table[i][j]) || std::isinf(table[i][j]))
            {
                order -= 1;
                if (order < 0)
                {
                    throw std::invalid_argument("Newton interpolation, invalid input array.\n");
                }
            }
        }
    }

    double res = 0.0;
    for (int i = 0; i <= order; i++)
    {
        double temp = table[i][i];
        for (int j = 0; j < i; j++)
        {
            temp *= (tarX - x[j]);
        }
        res += temp;
    }

    return res;
}
}