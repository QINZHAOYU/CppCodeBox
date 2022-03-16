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
using std::vector;

class IInterpolation
{
public:
    virtual bool CheckDuplicate(const vector<double> &x);
    virtual void SortCurve(vector<double> &x, vector<double> &y);
    virtual void RemoveDuplicateAfterSort(vector<double> &x, vector<double> &y);
    virtual void RemoveElements(vector<double> &arr, vector<int> &elemIndexes);

    virtual void   Clear()                                                       = 0;
    virtual void   SetRawCurve(const vector<double> &x, const vector<double> &y) = 0;
    virtual void   SetRawCurve(double x[], double y[], int num)                  = 0;
    virtual void   CheckInterpolationOrder(int &order)                           = 0;
    virtual double Interpolate(double tarX, int order)                           = 0;
};


class NewtonInterpolation : public IInterpolation
{
public:
    NewtonInterpolation();

    void Clear() override;
    void SetRawCurve(const vector<double> &x, const vector<double> &y) override;
    void SetRawCurve(double x[], double y[], int num) override;

    void   CheckInterpolationOrder(int &order) override;
    double Interpolate(double tarX, int order) override;

private:
    void GenerateDerivative();
    int  FindInterpolateRangeStartIndex(double tarX, int order);

private:
    bool                   _hasDerivative = false;
    vector<vector<double>> _rawCurve;
    vector<vector<double>> _derivative;
};

} // namespace ccb