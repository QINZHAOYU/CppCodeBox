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

/// \brief To search shortest path between two vertices in directed graph.
class DirectedGraphHandler
{
public:
	using GraphMatrix = vector<vector<double>>; // adjacent matrix of graph.
	using Graph = vector<tuple<string, string, double>>;

	bool setGraph(const Graph &graph);
	bool runDijkstraAlgo(const string &begVertex, const string &endVertex);
	bool runDijkstraAlgo(const vector<array<string, 2>> &vertices);

	void getGraphMatrix(GraphMatrix &matrix) const;
	void displayGraphMatrix() const;

private:
	bool isGraphConnected();
	string getVertexId(int ind) const;

private:
	VecDbl             _dist;         // nothing1.
	VecBool            _book;
	VecInt             _path;         // nothing2.
	GraphMatrix        _matrix;
	MapStrInt          _verIdToInd;
};
