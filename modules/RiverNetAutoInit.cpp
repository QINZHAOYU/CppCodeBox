/** *****************************************************************************
*    @File      :  RiverNetAutoInit.cpp
*    @Brief     :  To provide an automatically initialization method of river net.
*
** ******************************************************************************/

#include "RiverNetAutoInit.hpp"
#include "common/CommConsts.hpp"
#include <set>
#include <iomanip>


/////////////////////////////////////////////////////////////////////////////////
// class DirectedGraphHandler.
/////////////////////////////////////////////////////////////////////////////////

bool DirectedGraphHandler::setGraph(const Graph &graph)
{
    // collect all vertices and map it.
    std::set<string> vertices;
    std::for_each(graph.begin(), graph.end(), [&vertices](const auto &iter) {
        vertices.emplace(std::get<0>(iter));
        vertices.emplace(std::get<1>(iter));
    });
    
    // map vertices.
    int ind = 0;
    std::for_each(vertices.begin(), vertices.end(), [this, &ind](const auto &iter) {
        _verIdToInd.emplace(iter, ind++);
    });

    // generate adjacent matrix of directed graph:
    // 1. distance between a vertex and itself is 0.0 ;
    // 2. distance between unconnected vertices is INF;
    // 3. distance between connected vertices is loaded.
    _matrix = GraphMatrix(vertices.size(), VecDbl(vertices.size(), _DBL_MAX));
    for(int i = 0; i < vertices.size() ; ++i)  
    {
        _matrix[i][i] = 0.0;
    }
    std::for_each(graph.begin(), graph.end(), [this](const auto &iter){
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

void DirectedGraphHandler::displayGraphMatrix() const
{
    cout << endl << std::setw(12) << "--";
    for (int i = 0; i < _verIdToInd.size() ; ++i)
    {
        cout << std::setw(6) << getVertexId(i) ;
    }
    cout << endl;
    for (int i = 0; i < _verIdToInd.size() ; ++i)
    {
        cout << std::setw(12) << getVertexId(i) ;
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

string DirectedGraphHandler::getVertexId(int ind) const
{
    auto iter = std::find_if(_verIdToInd.begin(), _verIdToInd.end(), 
    [ind](const auto &elem){return elem.second == ind;}
    );

    return (iter != _verIdToInd.end()) ? iter->first : "";
}








