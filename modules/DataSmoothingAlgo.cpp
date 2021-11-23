/** *****************************************************************************
*    @File      :  DataSmoothingAlgo.cpp
*    @Brief     :  To smooth data series.
*
** ******************************************************************************/

#include "DataSmoothingAlgo.hpp"
#include <numeric>


namespace ccb
{

/////////////////////////////////////////////////////////////////////////////////
// class DataSmoother.
/////////////////////////////////////////////////////////////////////////////////

void DataSmoother::linearSmoothN3(const vector<double> &orig, vector<double> &res)
{
	res.clear();

	// return origin data series.
	size_t size = orig.size();
	if (size < 3)
	{
		res = orig;
		return;
	}

	// left boundary element.
	double elem = (5.0 * orig[0] + 2.0 * orig[1] - orig[2]) / 6.0;
	res.push_back(elem);

	// linear smoothing by 3-points average.
	for (int i = 1; i < size - 1; ++i)
	{
		elem = std::accumulate(orig.begin() + i - 1, orig.begin() + i + 1, 0.0) / 3.0;
		res.push_back(elem);
	}

	// right boundary element.
	elem = (5.0 * orig[size - 1] + 2.0 * orig[size - 2] - orig[size - 3]) / 6.0;
	res.push_back(elem);
}

void DataSmoother::linearSmoothN5(const vector<double> &orig, vector<double> &res)
{
	res.clear();

	// return origin data series.
	size_t size = orig.size();
	if (size < 5)
	{
		res = orig;
		return;
	}

	// smooth calculation.
	VecDbl args0 = {3.0, 2.0, 1.0, 0.0, -1.0, 5.0};
	VecDbl args1 = {4.0, 3.0, 2.0, 1.0, 0.0, 10.0};
	VecDbl elems;

	// left boundary element.
	elems.assign(orig.begin(), orig.begin() + 5);
	res.push_back(smoother(elems, args0));
	res.push_back(smoother(elems, args1));

	// linear smoothing by 5-points average.
	for (int i = 2; i < size - 2; ++i)
	{
		elems.assign(orig.begin() + i - 2, orig.begin() + i + 3);
		res.push_back(smoother(elems));
	}

	// right boundary element.
	elems = reverseVec(VecDbl(orig.end() - 5, orig.end()));
	res.push_back(smoother(elems, args1));
	res.push_back(smoother(elems, args0));
}

void DataSmoother::linearSmoothN7(const vector<double> &orig, vector<double> &res)
{
	res.clear();

	// return origin data series.
	size_t size = orig.size();
	if (size < 7)
	{
		res = orig;
		return;
	}

	// smooth calculation.
	VecDbl args0 = {13.0, 10.0, 7.0, 4.0, 1.0, -2.0, -5.0, 28.0};
	VecDbl args1 = {5.0, 4.0, 3.0, 2.0, 1.0, 0.0, -1.0, 14.0};
	VecDbl args2 = {7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 28.0};
	VecDbl elems;

	// left boundary element.
	elems.assign(orig.begin(), orig.begin() + 7);
	res.push_back(smoother(elems, args0));
	res.push_back(smoother(elems, args1));
	res.push_back(smoother(elems, args2));

	// linear smoothing by 7-points average.
	for (int i = 3; i < size - 3; ++i)
	{
		elems.assign(orig.begin() + i - 3, orig.begin() + i + 4);
		res.push_back(smoother(elems));
	}

	// right boundary element.
	elems = reverseVec(VecDbl(orig.end() - 7, orig.end()));
	res.push_back(smoother(elems, args2));
	res.push_back(smoother(elems, args1));
	res.push_back(smoother(elems, args0));
}

void DataSmoother::quadraticSmoothN5(const vector<double> &orig, vector<double> &res)
{
	res.clear();

	// return origin data series.
	size_t size = orig.size();
	if (size < 5)
	{
		res = orig;
		return;
	}

	// smooth calculation.
	VecDbl args0 = {31.0, 9.0, -3.0, -5.0, 3.0, 35.0};
	VecDbl args1 = {9.0, 13.0, 12.0, 6.0, -5.0, 35.0};
	VecDbl args2 = {-3.0, 12.0, 17.0, 1.0, 1.0, 35.0};
	VecDbl elems;

	// left boundary element.
	elems.assign(orig.begin(), orig.begin() + 5);
	res.push_back(smoother(elems, args0));
	res.push_back(smoother(elems, args1));

	// quadratic smoothing by 5-points average.
	for (int i = 2; i < size - 2; ++i)
	{
		elems.assign(orig.begin() + i - 2, orig.begin() + i + 3);
		res.push_back(smoother(elems, args2));
	}

	// right boundary element.
	elems = reverseVec(VecDbl(orig.end() - 5, orig.end()));
	res.push_back(smoother(elems, args1));
	res.push_back(smoother(elems, args0));
}

void DataSmoother::quadraticSmoothN7(const vector<double> &orig, vector<double> &res)
{
	res.clear();

	// return origin data series.
	size_t size = orig.size();
	if (size < 7)
	{
		res = orig;
		return;
	}

	// smooth calculation.
	VecDbl args0 = {32.0, 15.0, 3.0, -4.0, -6.0, -3.0, 5.0, 42.0};
	VecDbl args1 = {5.0, 4.0, 3.0, 2.0, 1.0, 0.0, -1.0, 14.0};
	VecDbl args2 = {1.0, 3.0, 4.0, 4.0, 3.0, 1.0, -2.0, 14.0};
	VecDbl args3 = {-2.0, 3.0, 6.0, 7.0, 6.0, 3.0, -2.0, 21.0};
	VecDbl elems;

	// left boundary element.
	elems.assign(orig.begin(), orig.begin() + 7);
	res.push_back(smoother(elems, args0));
	res.push_back(smoother(elems, args1));
	res.push_back(smoother(elems, args2));

	// quadratic smoothing by 7-points average.
	for (int i = 3; i < size - 3; ++i)
	{
		elems.assign(orig.begin() + i - 3, orig.begin() + i + 4);
		res.push_back(smoother(elems, args3));
	}

	// right boundary element.
	elems = reverseVec(VecDbl(orig.end() - 7, orig.end()));
	res.push_back(smoother(elems, args2));
	res.push_back(smoother(elems, args1));
	res.push_back(smoother(elems, args0));
}

void DataSmoother::cubicSmoothN5(const vector<double> &orig, vector<double> &res)
{
	res.clear();

	// return origin data series.
	size_t size = orig.size();
	if (size < 5)
	{
		res = orig;
		return;
	}

	// smooth calculation.
	VecDbl args0 = {69.0, 4.0, -6.0, 4.0, -1.0, 70.0};
	VecDbl args1 = {2.0, 27.0, 12.0, -8.0, 2.0, 35.0};
	VecDbl args2 = {-3.0, 12.0, 17.0, 12.0, -3.0, 35.0};
	VecDbl elems;

	// left boundary element.
	elems.assign(orig.begin(), orig.begin() + 5);
	res.push_back(smoother(elems, args0));
	res.push_back(smoother(elems, args1));

	// cubic smoothing by 5-points average.
	for (int i = 2; i < size - 2; ++i)
	{
		elems.assign(orig.begin() + i - 2, orig.begin() + i + 3);
		res.push_back(smoother(elems, args2));
	}

	// right boundary element.
	elems = reverseVec(VecDbl(orig.end() - 5, orig.end()));
	res.push_back(smoother(elems, args1));
	res.push_back(smoother(elems, args0));
}

void DataSmoother::cubicSmoothN7(const vector<double> &orig, vector<double> &res)
{
	res.clear();

	// return origin data series.
	size_t size = orig.size();
	if (size < 7)
	{
		res = orig;
		return;
	}

	// smooth calculation.
	VecDbl args0 = {39.0, 8.0, -4.0, -4.0, 1.0, 4.0, -2.0, 42.0};
	VecDbl args1 = {8.0, 19.0, 16.0, 6.0, -4.0, -7.0, 4.0, 42.0};
	VecDbl args2 = {-4.0, 16.0, 19.0, 12.0, 2.0, -4.0, 1.0, 42.0};
	VecDbl args3 = {-2.0, 3.0, 6.0, 7.0, 6.0, 3.0, -2.0, 21.0};
	VecDbl elems;

	// left boundary element.
	elems.assign(orig.begin(), orig.begin() + 7);
	res.push_back(smoother(elems, args0));
	res.push_back(smoother(elems, args1));
	res.push_back(smoother(elems, args2));

	// cubic smoothing by 7-points average.
	for (int i = 3; i < size - 3; ++i)
	{
		elems.assign(orig.begin() + i - 3, orig.begin() + i + 4);
		res.push_back(smoother(elems, args3));
	}

	// right boundary element.
	elems = reverseVec(VecDbl(orig.end() - 7, orig.end()));
	res.push_back(smoother(elems, args2));
	res.push_back(smoother(elems, args1));
	res.push_back(smoother(elems, args0));
}

double DataSmoother::smoother(const VecDbl &elems, const VecDbl &args)
{
	if (args.size() != elems.size() + 1)
	{
		std::cerr << "error: size of elems and args not match" << _LOCA;
		return 0.0;
	}

	double res = 0.0;
	for (int i = 0; i < elems.size(); ++i)
	{
		res += args[i] * elems[i];
	}

	return res / args.back();
}

double DataSmoother::smoother(const VecDbl &elems)
{
	if (elems.empty())
	{
		std::cerr << "error: smooth empty elements " << _LOCA;
		return 0.0;
	}

	return std::accumulate(elems.begin(), elems.end(), 0.0) / elems.size();
}

VecDbl DataSmoother::reverseVec(VecDbl &&elems)
{
	std::reverse(elems.begin(), elems.end());
	return elems;
}

}  // end of namespace ccb.