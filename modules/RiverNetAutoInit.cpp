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
	auto calc0 = [](double e0, double e1, double e2, double e3, double e4)
	{
		return (3.0 * e0 + 2.0 * e1 + e2 - e4) / 5.0;
	};
	auto calc1 = [](double e0, double e1, double e2, double e3, double e4)
	{
		return (4.0 * e0 + 3.0 * e1 + 2.0 * e2 + e3) / 10.0;
	};
	auto calc2 = [](const VecDbl && elems)
	{
		return std::accumulate(elems.begin(), elems.end(), 0.0) / elems.size();
	};

	// left boundary element.
	res.push_back(calc0(orig[0], orig[1], orig[2], orig[3], orig[4]));
	res.push_back(calc1(orig[0], orig[1], orig[2], orig[3], orig[4]));

	// linear smoothing by 5-points average.
	for (int i = 2; i < size - 2; ++i)
	{
		double elem = calc2(VecDbl(orig.begin() + i - 2, orig.begin() + i + 2));
		res.push_back(elem);
	}

	// right boundary element.
	res.push_back(calc1(orig[size - 1], orig[size - 2], orig[size - 3],
	                    orig[size - 4], orig[size - 5]));
	res.push_back(calc0(orig[size - 1], orig[size - 2], orig[size - 3],
	                    orig[size - 4], orig[size - 5]));
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
	auto calc0 = [](const VecDbl && elems)
	{
		return (13.0 * elems[0] + 10.0 * elems[1] + 7.0 * elems[2] + 4.0 * elems[3]
		        + elems[4] - 2.0 * elems[5] - 5.0 * elems[6]) / 28.0;
	};
	auto calc1 = [](const VecDbl && elems)
	{
		return (5.0 * elems[0] + 4.0 * elems[1] + 3.0 * elems[2] + 2.0 * elems[3]
		        + elems[4] - elems[6]) / 14.0;
	};
	auto calc2 = [](const VecDbl && elems)
	{
		return (7.0 * elems[0] + 6.0 * elems[1] + 5.0 * elems[2] + 4.0 * elems[3]
		        + 3.0 * elems[4] + 2.0 * elems[5] + elems[6]) / 28.0;
	};
	auto calc3 = [](const VecDbl && elems)
	{
		return std::accumulate(elems.begin(), elems.end(), 0.0) / elems.size();
	};

	// left boundary element.
	res.push_back(calc0(VecDbl(orig.begin(), orig.begin() + 7)));
	res.push_back(calc1(VecDbl(orig.begin(), orig.begin() + 7)));
	res.push_back(calc2(VecDbl(orig.begin(), orig.begin() + 7)));

	// linear smoothing by 7-points average.
	for (int i = 3; i < size - 3; ++i)
	{
		double elem = calc3(VecDbl(orig.begin() + i - 3, orig.begin() + i + 3));
		res.push_back(elem);
	}

	// right boundary element.
	VecDbl elems(orig.end() - 7, orig.end());
	std::reverse(elems.begin(), elems.end());
	res.push_back(calc2(move(elems)));
	res.push_back(calc1(move(elems)));
	res.push_back(calc0(move(elems)));
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
	auto calc0 = [](const VecDbl && elems)
	{
		return (31.0 * elems[0] + 9.0 * elems[1] - 3.0 * elems[2] - 5.0 * elems[3]
		        + 3.0 * elems[4]) / 35.0;
	};
	auto calc1 = [](const VecDbl && elems)
	{
		return (9.0 * elems[0] + 13.0 * elems[1] + 12.0 * elems[2] + 6.0 * elems[3]
		        - 5.0 * elems[4]) / 35.0;
	};
	auto calc2 = [](const VecDbl && elems)
	{
		return (-3.0 * elems[0] + elems[4] + 12.0 * elems[1] + elems[3]
		        + 17.0 * elems[2]) / 35.0;
	};

	// left boundary element.
	res.push_back(calc0(VecDbl(orig.begin(), orig.begin() + 5)));
	res.push_back(calc1(VecDbl(orig.begin(), orig.begin() + 5)));

	// quadratic smoothing by 5-points average.
	for (int i = 2; i < size - 2; ++i)
	{
		double elem = calc2(VecDbl(orig.begin() + i - 2, orig.begin() + i + 2));
		res.push_back(elem);
	}

	// right boundary element.
	VecDbl elems(orig.end() - 5, orig.end());
	std::reverse(elems.begin(), elems.end());
	res.push_back(calc1(move(elems)));
	res.push_back(calc0(move(elems)));
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
	auto calc0 = [](const VecDbl && elems)
	{
		return (32.0 * elems[0] + 15.0 * elems[1] + 3.0 * elems[2] - 4.0 * elems[3]
		        - 6.0 * elems[4] - 3.0 * elems[5] + 5.0 * elems[6] ) / 42.0;
	};
	auto calc1 = [](const VecDbl && elems)
	{
		return (5.0 * elems[0] + 4.0 * elems[1] + 3.0 * elems[2] + 2.0 * elems[3]
		        + elems[4] - elems[6] ) / 14.0;
	};
	auto calc2 = [](const VecDbl && elems)
	{
		return (elems[0] + 3.0 * elems[1] + 4.0 * elems[2] + 4.0 * elems[3]
		        + 3.0 * elems[4] + 1.0 * elems[5] - 2.0 * elems[6] ) / 14.0;
	};
	auto calc3 = [](const VecDbl && elems)
	{
		return (-2.0 * (elems[0] + elems[6]) + 3.0 * (elems[1] + elems[5])
		        + 6.0 * (elems[2] + elems[4]) + 7.0 * elems[3] ) / 21.0;
	};

	// left boundary element.
	res.push_back(calc0(VecDbl(orig.begin(), orig.begin() + 7)));
	res.push_back(calc1(VecDbl(orig.begin(), orig.begin() + 7)));
	res.push_back(calc2(VecDbl(orig.begin(), orig.begin() + 7)));

	// quadratic smoothing by 7-points average.
	for (int i = 3; i < size - 3; ++i)
	{
		double elem = calc3(VecDbl(orig.begin() + i - 3, orig.begin() + i + 3));
		res.push_back(elem);
	}

	// right boundary element.
	VecDbl elems(orig.end() - 7, orig.end());
	std::reverse(elems.begin(), elems.end());
	res.push_back(calc2(move(elems)));
	res.push_back(calc1(move(elems)));
	res.push_back(calc0(move(elems)));
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
	auto calc0 = [](const VecDbl && elems)
	{
		return (69.0 * elems[0] + 4.0 * elems[1] - 6.0 * elems[2] + 4.0 * elems[3]
		        - elems[4]) / 70.0;
	};
	auto calc1 = [](const VecDbl && elems)
	{
		return (2.0 * elems[0] + 27.0 * elems[1] + 12.0 * elems[2] - 8.0 * elems[3]
		        + 2.0 * elems[4]) / 35.0;
	};
	auto calc2 = [](const VecDbl && elems)
	{
		return (-3.0 * (elems[0] + elems[4]) + 12.0 * (elems[1] + elems[3])
		        + 17.0 * elems[2] ) / 35.0;
	};

	// left boundary element.
	res.push_back(calc0(VecDbl(orig.begin(), orig.begin() + 5)));
	res.push_back(calc1(VecDbl(orig.begin(), orig.begin() + 5)));

	// cubic smoothing by 5-points average.
	for (int i = 2; i < size - 2; ++i)
	{
		double elem = calc2(VecDbl(orig.begin() + i - 2, orig.begin() + i + 2));
		res.push_back(elem);
	}

	// right boundary element.
	VecDbl elems(orig.end() - 5, orig.end());
	std::reverse(elems.begin(), elems.end());
	res.push_back(calc1(move(elems)));
	res.push_back(calc0(move(elems)));
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
	auto calc0 = [](const VecDbl && elems)
	{
		return (39.0 * elems[0] + 8.0 * elems[1] - 4.0 * elems[2] - 4.0 * elems[3]
		        + 1.0 * elems[4] + 4.0 * elems[5] - 2.0 * elems[6] ) / 42.0;
	};
	auto calc1 = [](const VecDbl && elems)
	{
		return (8.0 * elems[0] + 19.0 * elems[1] + 16.0 * elems[2] + 6.0 * elems[3]
		        - 4.0 * elems[4] - 7.0 * elems[5] + 4.0 * elems[6] ) / 42.0;
	};
	auto calc2 = [](const VecDbl && elems)
	{
		return (-4.0 * elems[0] + 16.0 * elems[1] + 19.0 * elems[2] + 12.0 * elems[3]
		        + 2.0 * elems[4] - 4.0 * elems[5] + 1.0 * elems[6] ) / 42.0;
	};
	auto calc3 = [](const VecDbl && elems)
	{
		return (-2.0 * (elems[0] + elems[6]) + 3.0 * (elems[1] + elems[5])
		        + 6.0 * (elems[2] + elems[4]) + 7.0 * elems[3] ) / 21.0;
	};

	// left boundary element.
	res.push_back(calc0(VecDbl(orig.begin(), orig.begin() + 7)));
	res.push_back(calc1(VecDbl(orig.begin(), orig.begin() + 7)));
	res.push_back(calc2(VecDbl(orig.begin(), orig.begin() + 7)));

	// cubic smoothing by 7-points average.
	for (int i = 3; i < size - 3; ++i)
	{
		double elem = calc3(VecDbl(orig.begin() + i - 3, orig.begin() + i + 3));
		res.push_back(elem);
	}

	// right boundary element.
	VecDbl elems(orig.end() - 7, orig.end());
	std::reverse(elems.begin(), elems.end());
	res.push_back(calc2(move(elems)));
	res.push_back(calc1(move(elems)));
	res.push_back(calc0(move(elems)));
}










