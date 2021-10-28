/** *****************************************************************************
*    @File      :  RiverNetAutoInit.cpp
*    @Brief     :  To provide an automatically initialization method of river net.
*
** ******************************************************************************/

#include "RiverNetAutoInit.hpp"
#include "common/CommConsts.hpp"
#include <set>
#include <iomanip>
#include <exception>


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

    for (const auto &route: _pathRoutes)
    {
        VecStr path;
        for (const auto &ver: route)
        {
            path.emplace_back(getVerticeId(ver));
        }

        pathes.emplace_back(path);
    }
}

void DirectedGraphHandler::getCurrVertices(array<string, 2> &vertices) const
{
    vertices[0] = getVerticeId(_currBegVerticeInd);
    vertices[1] = getVerticeId(_currEndVerticeInd);
}

void DirectedGraphHandler::getCurrPath(VecStr &path) const
{
    path.clear();

    if (_pathRoutes.empty())
    {
        return;
    }

    for (const auto &ver: _pathRoutes.back())
    {
        path.emplace_back(getVerticeId(ver));
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
    for (const auto &vers: vertices)
    {
        if (!runDijkstraAlgo(vers[0], vers[1]))
        {
            return false;  // return `false` once Dijkstra algorithm failed.
        }
    }

    return true;
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
    catch(const std::exception& e)
    {
        std::cerr << e.what() << endl;
    }

    _matrix = matrix;  // recover graph matrix.
    if (_path[_currEndVerticeInd] >= 0) // the shortest path found.
    {
        _pathRoutes.emplace_back(parsePath()); 
        return true;
    }
    else
    {
        clearCurrStatus(); 
        return false;
    }   
}

bool DirectedGraphHandler::checkDijkstraAlgoValid(const string &begVertice, 
const string &endVertice)
{
    if (_verIdToInd.count(begVertice) < 1 || _verIdToInd.count(endVertice) < 1)
    {
        std::cerr << "invalided vertex: " << begVertice << " , " << endVertice
                  << __FILE__ << __LINE__ << endl;
        return false;
    }

    if (_matrix.empty())
    {
        std::cerr << "graph matrix is empty. " << __FILE__ << __LINE__ << endl;
        return false;
    }

    for (const auto &row: _matrix)
    {
        auto iter = std::find_if(row.begin(), row.end(), [](const double val)
        {
            return val < 0.0;
        });

        if (iter != row.end())
        {
            std::cerr << "invalided graph: has negative edge value." 
                      << __FILE__ << __LINE__ << endl;
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
        ?_path.emplace_back(_currBegVerticeInd)
        :_path.emplace_back(-1);
    });
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
    while(preVerInd != _currBegVerticeInd)
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
    for (const auto &route: _pathRoutes)
    {
        for (const auto &ver: route)
        {
            cout << " -> " << getVerticeId(ver) ;
        }
        cout << endl << endl;
    }
}



