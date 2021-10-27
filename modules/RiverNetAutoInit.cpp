/** *****************************************************************************
*    @File      :  RiverNetAutoInit.cpp
*    @Brief     :  To provide an automatically initialization method of river net.
*
** ******************************************************************************/

# include "RiverNetAutoInit.hpp"
# include "common/CommConsts.hpp"
# include <set>


/////////////////////////////////////////////////////////////////////////////////
// class DirectedGraphHandler.
/////////////////////////////////////////////////////////////////////////////////

bool DirectedGraphHandler::setGraph(const Graph& graph)
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

    // generate adjacent matrix:
    // 1. distence between a vertex and itself is 0.0 ;
    // 2. distence between unconnected vertices is INF;
    // 3. distence between connected vertices is loaded.
    _matrix = GraphMatrix(vertices.size(), VecDbl(vertices.size(), _DBL_MAX));
    for(int i = 0; i < vertices.size() ; ++i)  
    {
        _matrix[i][i] = 0.0;
    }
    std::for_each(graph.begin(), graph.end(), [this](const auto &iter){
        string ver1, ver2;
        double dist;
        std::tie(ver1, ver2, dist) = iter;
        _matrix[_verIdToInd[ver1]][_verIdToInd[ver2]] = dist;
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

















