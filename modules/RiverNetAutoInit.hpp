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
///
/// \attention Dijkstra algorithm has the ability to search all pathes once from
///        source vertice to all others. Meanwhile, Dijkstra algorithm doesn't
///        support negative edge value(weight).
class DirectedGraphHandler
{
public:
	using GraphMatrix = vector<vector<double>>;  ///< graph adjacent matrix.
	using Graph = vector<tuple<string, string, double>>;  ///< graph edges.

	bool setGraph(const Graph &graph);
	bool runDijkstraAlgo(const string &begVertice, const string &endVertice);
	bool runDijkstraAlgo(const vector<array<string, 2>> &vertices);

	void getGraphMatrix(GraphMatrix &matrix) const;
	void getPathes(vector<VecStr> &pathes) const;

	void displayGraphMatrix() const;
	void displayPathes() const;

private:
	string getVerticeId(int ind) const;
	bool isGraphConnected();
	void clearCurrStatus();

	bool checkDijkstraAlgoValid(const string &begVertice, const string &endVertice);
	void initDijkstraAlgoStatus();
	void DijkstraAlgo();
	VecInt parsePath();

private:
	int                _currBegVerticeInd;  // current begin vertice index.
	int                _currEndVerticeInd;  // current end vertice index.
	bool               _isMultiSets{false}; // if inputting many vertices once for Dijkstra.
	vector<VecInt>     _pathRoutes;         // shortest pathes consisting of vertices index.

	VecDbl             _dist;               // path length from source vertice to all others.
	VecBool            _book;               // vertices with known shortest path.
	VecInt             _path;               // previous vertice in shortest path of each vertices.
	GraphMatrix        _matrix;             // graph matrix recoding path length.
	MapStrInt          _verIdToInd;         // map of vertices and its index.
};


/// \brief To smooth data series.
class DataSmoother
{
public:
	static void linearSmoothN3(const vector<double> &orig, vector<double> &res);
	static void linearSmoothN5(const vector<double> &orig, vector<double> &res);
	static void linearSmoothN7(const vector<double> &orig, vector<double> &res);

    // error.
	static void quadraticSmoothN5(const vector<double> &orig, vector<double> &res);
	static void quadraticSmoothN7(const vector<double> &orig, vector<double> &res);

	static void cubicSmoothN5(const vector<double> &orig, vector<double> &res);
	static void cubicSmoothN7(const vector<double> &orig, vector<double> &res);
};


/// \brief To generate and parse river net directed graph.
class RiverNetGrapher
{
public:
private:
};


/// \brief To handle river net status(water level and discharge).
class RiverNetStatusHandler
{
public:
private:
};


/// \brief To export class pointer.
DLLAPI DirectedGraphHandler* getDirectedGraphHandler();
DLLAPI DataSmoother* getDataSmoother();

