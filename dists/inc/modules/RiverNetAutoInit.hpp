/** *****************************************************************************
*   @copyright :  Copyright (C) 2021 Qin ZhaoYu. All rights reserved.
*
*   @author    :  Qin ZhaoYu.
*   @see       :  https://github.com/QINZHAOYU
*   @brief     :  To provide an automatically initialization method of river net.
*
*   Change History:
*   -----------------------------------------------------------------------------
*   --version-1.0, 2021/10/22, Qin ZhaoYu, zhaoyu.qin@foxmail.com
*   Init model.
*
** ******************************************************************************/

#pragma once
#include "common/CommHeader.hpp"
#include "common/CommStructs.hpp"

// for test.
#include <iomanip>


/// \brief To search shortest path between two vertices in directed graph.
class DirectedGraphHandler
{
public:
    using GraphMatrix = vector<vector<double>>;  // adjacent matrix of graph. 
    using Graph = vector<tuple<string, string, double>>;

    bool setGraph(const Graph& graph);


    void getGraphMatrix(GraphMatrix &matrix) const;
    void displayGraphMatrix() const;

private:
    bool isGraphConnected();
    string getVertexId(int ind) const;


private:
    VecDbl                 _dist;
    VecBool                _book;
    VecInt                 _path;
    GraphMatrix            _matrix;
    MapStrInt              _verIdToInd;
};















