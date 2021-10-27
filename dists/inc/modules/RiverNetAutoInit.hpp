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

    void test_displayGraphMatrix()
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
                if (_matrix[i][j] > 100) 
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

private:
    bool isGraphConnected();
    string getVertexId(int ind)
    {
        auto iter = std::find_if(_verIdToInd.begin(), _verIdToInd.end(), [ind](const auto &elem){ 
            return elem.second == ind;
        });

        if (iter != _verIdToInd.end())
        {
            return iter->first;
        }
        else
        {
            return "";
        }
    }

private:
    VecDbl                 _dist;
    VecBool                _book;
    VecInt                 _path;
    GraphMatrix            _matrix;
    MapStrInt              _verIdToInd;
};















