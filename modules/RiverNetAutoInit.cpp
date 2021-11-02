/** *****************************************************************************
*    @File      :  RiverNetAutoInit.cpp
*    @Brief     :  To provide an automatically initialization method of river net.
*
** ******************************************************************************/

#include "RiverNetAutoInit.hpp"
#include "common/CommConsts.hpp"
#include <set>
#include <iomanip>
#include <numeric>


/////////////////////////////////////////////////////////////////////////////////
// class DirectedGraphHandler.
/////////////////////////////////////////////////////////////////////////////////

bool DirectedGraphHandler::setGraph(const Graph &graph)
{
	// collect all vertices and map it.
	std::set<string> vertices;
	std::for_each(graph.begin(), graph.end(), [&vertices](const auto & iter)
	{
		vertices.emplace(std::get<0>(iter));
		vertices.emplace(std::get<1>(iter));
	});

	// map vertices.
	int ind = 0;
	std::for_each(vertices.begin(), vertices.end(), [this, &ind](const auto & iter)
	{
		_verIdToInd.emplace(iter, ind++);
	});

	// generate adjacent matrix of directed graph:
	// 1. distance between a vertice and itself is 0.0 ;
	// 2. distance between unconnected vertices is INF;
	// 3. distance between connected vertices is loaded.
	_matrix = GraphMatrix(vertices.size(), VecDbl(vertices.size(), _DBL_MAX));
	for (int i = 0; i < vertices.size() ; ++i)
	{
		_matrix[i][i] = 0.0;
	}
	std::for_each(graph.begin(), graph.end(), [this](const auto & iter)
	{
		int begInd = _verIdToInd[std::get<0>(iter)];
		int endInd = _verIdToInd[std::get<1>(iter)];
		_matrix[begInd][endInd] = std::get<2>(iter);
	});

	return isGraphConnected();
}

bool DirectedGraphHandler::isGraphConnected()
{
	//===========================================================================
	// Assume the graph inputed is connected for the moment.
	//===========================================================================

	return true;
}

void DirectedGraphHandler::getGraphMatrix(GraphMatrix &matrix) const
{
	matrix = _matrix;
}

void DirectedGraphHandler::getPathes(vector<VecStr> &pathes) const
{
	pathes.clear();

	for (const auto &route : _pathRoutes)
	{
		VecStr path;
		for (const auto &ver : route)
		{
			path.emplace_back(getVerticeId(ver));
		}

		pathes.emplace_back(path);
	}
}

string DirectedGraphHandler::getVerticeId(int ind) const
{
	auto iter = std::find_if(_verIdToInd.begin(), _verIdToInd.end(),
	[ind](const auto & elem) {return elem.second == ind;});

	return (iter != _verIdToInd.end()) ? iter->first : "";
}

bool DirectedGraphHandler::runDijkstraAlgo(const vector<array<string, 2>> &vertices)
{
	_isMultiSets = true;
	bool status = true;

	for (const auto &vers : vertices)
	{
		if (!runDijkstraAlgo(vers[0], vers[1]))
		{
			status = false;  // return `false` once Dijkstra algorithm failed.
			break;
		}
	}

	_isMultiSets = false;
	return status;
}

bool DirectedGraphHandler::runDijkstraAlgo(const string &begVertice,
        const string &endVertice)
{
	if (!checkDijkstraAlgoValid(begVertice, endVertice))
	{
		clearCurrStatus();
		return false;
	}

	GraphMatrix matrix = _matrix;
	try
	{
		_currBegVerticeInd = _verIdToInd[begVertice];
		_currEndVerticeInd = _verIdToInd[endVertice];

		initDijkstraAlgoStatus();
		DijkstraAlgo();
	}
	catch (...)
	{
		std::cerr << "Dijkstra crashed" << _LOCA;
	}

	_matrix = matrix;  // recover graph matrix.
	if (_path[_currEndVerticeInd] >= 0) // the shortest path found.
	{
		_pathRoutes.emplace_back(parsePath());
		return true;
	}
	else
	{
		std::cerr << "no path found: "
		          << begVertice << " -> " << endVertice << _LOCA;

		clearCurrStatus();
		return false;
	}
}

bool DirectedGraphHandler::checkDijkstraAlgoValid(const string &begVertice,
        const string &endVertice)
{
	if (_verIdToInd.count(begVertice) < 1 || _verIdToInd.count(endVertice) < 1)
	{
		std::cerr << "invalid vertices: " << begVertice << " , " << endVertice
		          << _LOCA;
		return false;
	}

	if (_matrix.empty())
	{
		std::cerr << "empty graph matrix" << _LOCA;
		return false;
	}

	for (const auto &row : _matrix)
	{
		auto iter = std::find_if(row.begin(), row.end(), [](const double val)
		{
			return val < 0.0;
		});

		if (iter != row.end())
		{
			std::cerr << "invalid graph: has negative edge value" << _LOCA;
			return false;
		}
	}

	return true;
}

void DirectedGraphHandler::initDijkstraAlgoStatus()
{
	_dist = _matrix[_currBegVerticeInd];

	_book = VecBool(_matrix.size(), false);
	_book[_currBegVerticeInd] = true;

	_path.clear();
	std::for_each(_dist.begin(), _dist.end(), [this](double ds)
	{
		(ds < _DBL_MAX - _DBL_EPSILON)
		? _path.emplace_back(_currBegVerticeInd)
		: _path.emplace_back(-1);
	});

	if (!_isMultiSets)
	{
		_pathRoutes.clear();
	}
}

void DirectedGraphHandler::DijkstraAlgo()
{
	for (int i = 0; i < _matrix.size(); ++i)
	{
		if (i == _currBegVerticeInd)
		{
			continue;
		}

		int postVerInd = _currBegVerticeInd;
		double minDist = _DBL_MAX;

		// find the next vertice making up the shortest path.
		for (int j = 0; j < _matrix.size(); ++j)
		{
			if (!_book[j] && minDist > _dist[j] + _DBL_EPSILON)
			{
				minDist = _dist[j];
				postVerInd = j;
			}
		}

		// iterate over all other vertices to update distance.
		_book[postVerInd] = true;
		for (int j = 0; j < _matrix.size(); ++j)
		{
			if (!_book[j] && _dist[j] > _dist[postVerInd] + _matrix[postVerInd][j])
			{
				_dist[j] = _dist[postVerInd] + _matrix[postVerInd][j];
				_path[j] = postVerInd;
			}
		}

		// stop searching for end vertice done(otherwise, all pathes searched).
		if (i == _currEndVerticeInd)
		{
			break;
		}
	}
}

VecInt DirectedGraphHandler::parsePath()
{
	VecInt route{_currEndVerticeInd};

	int preVerInd = _currEndVerticeInd;
	while (preVerInd != _currBegVerticeInd)
	{
		preVerInd = _path[preVerInd];
		route.emplace_back(preVerInd);
	}

	std::reverse(route.begin(), route.end());
	return route;
}

void DirectedGraphHandler::clearCurrStatus()
{
	_path.clear();
	_book.clear();
	_dist.clear();
	_pathRoutes.clear();
	_currBegVerticeInd = -1;
	_currEndVerticeInd = -1;
}

void DirectedGraphHandler::displayGraphMatrix() const
{
	cout << endl << std::setw(12) << "--";
	for (int i = 0; i < _verIdToInd.size() ; ++i)
	{
		cout << std::setw(6) << getVerticeId(i) ;
	}
	cout << endl;
	for (int i = 0; i < _verIdToInd.size() ; ++i)
	{
		cout << std::setw(12) << getVerticeId(i) ;
		for (int j = 0; j < _verIdToInd.size(); ++j)
		{
			if (_matrix[i][j] >= _DBL_MAX - _DBL_EPSILON)
			{
				cout << std::setw(6) << "nan";
			}
			else
			{
				cout << std::setw(6) << _matrix[i][j];
			}
		}
		cout << endl << endl;
	}
}

void DirectedGraphHandler::displayPathes() const
{
	cout << endl << endl;
	for (const auto &route : _pathRoutes)
	{
		for (const auto &ver : route)
		{
			cout << " -> " << getVerticeId(ver) ;
		}
		cout << endl << endl;
	}
}


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
	return std::move(elems);
}




/////////////////////////////////////////////////////////////////////////////////
// export classes for libraries.
/////////////////////////////////////////////////////////////////////////////////

DirectedGraphHandler *getDirectedGraphHandler()
{
	return new DirectedGraphHandler();
}

DataSmoother *getDataSmoother()
{
	return new DataSmoother();
}



