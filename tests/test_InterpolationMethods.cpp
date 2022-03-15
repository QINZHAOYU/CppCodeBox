#include "common/CommConsts.hpp"
#include "tools/Catch/catch.hpp"
#include "modules/InterpolationMethods.hpp"
#include <math.h>
#include <random>
#include <fstream>

using namespace ccb;
using namespace std;

double cubic(double x)
{
    return -1 * x * x * x * x - 2.0 * x * x * x + 2.0 * x * x + 3.0 * x + 4.0;
}

TEST_CASE("test Interpolation methods")
{
    int n = 30;
    double dx = 0.2;
    vector<double> x, y, y2;
    default_random_engine e;
    uniform_real_distribution<double> noise(-1.0, 1.0);

    fstream result("raw_data.txt", ios::out);
    result << "x,   y,   y2\n";
    for (int i = 1; i <= n; ++i)
    {
        double currX = dx * i;
        double currY = cubic(currX);
        double currY2 = currY + noise(e);

        x.push_back(currX);
        y.push_back(currY);
        y2.push_back(currY2);

        result << currX << ", " << currY << ",  " << currY2 << "\n";
    }

    SECTION("test of sin(), order 2")
    {
        fstream outer("result_2nd.txt", ios::out);
        outer << "interX,  res,   res2  \n";

        vector<double> newx, res, res2;

        for (int i = 1; i <= n; ++i)
        {
            double currX = 0.5 * (x[i - 1] + x[i]);
            double currY = NewtonInterpolation(x.data(), y.data(), n, currX, 3);
            double currY2 = NewtonInterpolation(x.data(), y2.data(), n, currX, 3);

            newx.push_back(currX);
            res.push_back(currY);
            res2.push_back(currY2);

            outer << currX << ", " << currY << ",  " << currY2 << "\n";
        }
    }
}