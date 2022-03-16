/** *****************************************************************************
 *    @File      :  InterpolationMethods.cpp
 *    @Brief     :  一些常用的差值方法.
 *
 ** ******************************************************************************/
#include "InterpolationMethods.hpp"
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <limits>

static double EPSILON = 2 * (std::numeric_limits<double>::epsilon)();


namespace ccb
{


// class IInterpolation ---------------------------------------------------------

bool IInterpolation::CheckDuplicate(const vector<double> &x)
{
    if (x.empty() || x.size() < 2)
    {
        return false;
    }

    for (int i = 1; i < x.size(); ++i)
    {
        if (abs(x[i] - x[i - 1]) < EPSILON)
        {
            return true;
        }
    }
    return false;
}

void IInterpolation::SortCurve(vector<double> &x, vector<double> &y)

{
    if (x.size() != y.size())
    {
        throw std::invalid_argument("Two vectors has different size.\n");
    }
    if (x.empty())
    {
        return;
    }

    vector<vector<double>> temp;
    for (int i = 0; i < x.size(); ++i)
    {
        temp.emplace_back(vector<double> {x[i], double(i)});
    }
    std::sort(temp.begin(), temp.end());
    std::sort(x.begin(), x.end());

    vector<int> orders;
    for (int i = 0; i < temp.size(); ++i)
    {
        orders.push_back(temp[i][1]);
    }

    vector<double> sortedY;
    for (int i = 0; i < orders.size(); ++i)
    {
        sortedY.push_back(y[orders[i]]);
    }
    std::swap(y, sortedY);
}

void IInterpolation::RemoveDuplicateAfterSort(vector<double> &x, vector<double> &y)
{
    if (x.empty() || x.size() < 2)
    {
        return;
    }

    vector<double> dxVec(x.size());
    std::adjacent_difference(x.begin(), x.end(), dxVec.begin());

    vector<int> depulicatedIndex;
    for (int i = 1; i < dxVec.size(); ++i)
    {
        if (dxVec[i] < EPSILON)
            depulicatedIndex.push_back(i);
    }
    if (depulicatedIndex.empty())
    {
        return;
    }

    RemoveElements(x, depulicatedIndex);
    RemoveElements(y, depulicatedIndex);
}

void IInterpolation::RemoveElements(vector<double> &arr, vector<int> &indexes)
{
    std::sort(indexes.begin(), indexes.end());
    if (indexes.empty())
    {
        return;
    }
    if (indexes.back() >= arr.size())
    {
        printf("Index for removing is beyond array size.\n");
        return;
    }

    int offset = 0;
    for (auto index : indexes)
    {
        if (index < 0)
            continue;
        arr.erase(arr.begin() + index - offset);
        offset++;
    }
}


// class NewtonInterpolation ----------------------------------------------------

NewtonInterpolation::NewtonInterpolation()
{}

void NewtonInterpolation::Clear()
{
    _rawCurve.clear();
    _derivative.clear();
    _hasDerivative = false;
}

void NewtonInterpolation::SetRawCurve(const vector<double> &x, const vector<double> &y)
{
    if (!_rawCurve.empty())
    {
        printf(
            "Warning: curve has alread setted, this would override previous settings.\n");
        Clear();
    }

    vector<double> xCopy(x), yCopy(y);
    SortCurve(xCopy, yCopy);
    RemoveDuplicateAfterSort(xCopy, yCopy);

    if (xCopy.empty())
    {
        throw std::invalid_argument("Inputed curve is empty.\n");
    }

    _rawCurve.push_back(xCopy);
    _rawCurve.push_back(yCopy);

    _derivative.resize(xCopy.size());
    for (int i = 0; i < _derivative.size(); ++i)
    {
        _derivative[i].resize(xCopy.size());
    }
    _derivative[0].assign(yCopy.begin(), yCopy.end());
}

void NewtonInterpolation::SetRawCurve(double x[], double y[], int num)
{
    vector<double> xCopy(x, x + num), yCopy(y, y + num);
    SetRawCurve(xCopy, yCopy);
}

void NewtonInterpolation::GenerateDerivative()
{
    int size = _derivative.size();
    for (int i = 1; i < size; i++)
    {
        for (int j = i; j < size; j++)
        {
            _derivative[i][j] = (_derivative[i - 1][j] - _derivative[i - 1][j - 1])
                                / (_rawCurve[0][j] - _rawCurve[0][j - i]);
        }
    }
    _hasDerivative = true;
}

void NewtonInterpolation::CheckInterpolationOrder(int &order)
{
    if (order < 0)
    {
        char msg[512];
        sprintf(msg, "Newton interpolation, invalid order(%d)\n", order);
        throw std::invalid_argument(msg);
    }

    if (_rawCurve[0].size() <= order)
    {
        order = _rawCurve.size() - 1;
    }
}

int NewtonInterpolation::FindInterpolateRangeStartIndex(double tarX, int order)
{
    int index = -1;
    int size  = _rawCurve[0].size();
    for (int i = 0; i < size; ++i)
    {
        if (_rawCurve[0][i] >= tarX)
        {
            index = (std::max)(i - 1, 0);
            break;
        }
    }

    if (index < 0)
    {
        return size - order;
    }
    else if (index == 0)
    {
        return index;
    }

    int offset = int(order / 2);
    index -= offset;  // todo: 根据 `tarX` 到各点的距离设置偏移方向和偏移量。
    index = (std::max)(0, index);

    if (index + order > size)
    {
        index -= (index + order - size);
    }

    return (std::max)(0, index);
}

double NewtonInterpolation::Interpolate(double tarX, int order)
{
    CheckInterpolationOrder(order);

    if (!_hasDerivative)
    {
        GenerateDerivative();
    }

    int    offset = FindInterpolateRangeStartIndex(tarX, order);
    double res    = 0.0;
    for (int i = 0; i <= order; i++)
    {
        double temp = _derivative[i][i + offset];
        for (int j = 0; j < i; j++)
        {
            temp *= (tarX - _rawCurve[0][j + offset]);
        }
        res += temp;
    }

    return res;
}


}