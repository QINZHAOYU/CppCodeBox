#include "tools/Catch/catch.hpp"
#include "common/CommConsts.hpp"
#include "modules/RiverNetAutoInit.hpp"
#include <fstream>
#include <regex>
#include <numeric>


TEST_CASE("test class DirectedGraphHandler")
{
	DirectedGraphHandler::Graph graph;
	graph.emplace_back(tuple<string, string, double> {"ver4", "ver5", 13});
	graph.emplace_back(tuple<string, string, double> {"ver4", "ver6", 15});
	graph.emplace_back(tuple<string, string, double> {"ver1", "ver2", 1});
	graph.emplace_back(tuple<string, string, double> {"ver1", "ver3", 12});
	graph.emplace_back(tuple<string, string, double> {"ver2", "ver3", 9});
	graph.emplace_back(tuple<string, string, double> {"ver2", "ver4", 3});
	graph.emplace_back(tuple<string, string, double> {"ver3", "ver5", 5});
	graph.emplace_back(tuple<string, string, double> {"ver4", "ver3", 4});
	graph.emplace_back(tuple<string, string, double> {"ver5", "ver6", 4});

	DirectedGraphHandler grapher;
	grapher.setGraph(graph);

	//===================================================================
	//    --  ver1  ver2  ver3  ver4  ver5  ver6
	//  ver1     0     1    12   nan   nan   nan
	//
	//  ver2   nan     0     9     3   nan   nan
	//
	//  ver3   nan   nan     0   nan     5   nan
	//
	//  ver4   nan   nan     4     0    13    15
	//
	//  ver5   nan   nan   nan   nan     0     4
	//
	//  ver6   nan   nan   nan   nan   nan     0
	//===================================================================

	SECTION("test setGraph()")
	{
		DirectedGraphHandler::GraphMatrix matrix;
		grapher.getGraphMatrix(matrix);
		REQUIRE(matrix.size() == 6);
		REQUIRE(Approx(matrix[0][1]).margin(_DBL_EPSILON) == 1.0);
		REQUIRE(Approx(matrix[0][2]).margin(_DBL_EPSILON) == 12.0);
		REQUIRE(Approx(matrix[1][2]).margin(_DBL_EPSILON) == 9.0);
		REQUIRE(Approx(matrix[1][3]).margin(_DBL_EPSILON) == 3.0);
		REQUIRE(Approx(matrix[2][4]).margin(_DBL_EPSILON) == 5.0);
		REQUIRE(Approx(matrix[3][2]).margin(_DBL_EPSILON) == 4.0);
		REQUIRE(Approx(matrix[3][4]).margin(_DBL_EPSILON) == 13.0);
		REQUIRE(Approx(matrix[3][5]).margin(_DBL_EPSILON) == 15.0);
		REQUIRE(Approx(matrix[4][5]).margin(_DBL_EPSILON) == 4.0);
		REQUIRE(Approx(matrix[5][5]).margin(_DBL_EPSILON) == 0.0);
	}

	vector<array<string, 2>> scenes =
	{
		{"ver1", "ver2"},
		{"ver1", "ver3"},
		{"ver1", "ver6"},
		{"ver2", "ver5"},
		{"ver4", "ver6"},
	};
	array<string, 2> scene1 =  {"ver1", "ver6"};
	array<string, 2> scene2 =  {"ver1", "ver3"};
	array<string, 2> scene3 =  {"ver3", "ver1"};
	array<string, 2> scene4 =  {"ver1", "ver7"};

	//===================================================================
	// ver1->ver2: -> ver1 -> ver2
	//
	// ver1->ver3: -> ver1 -> ver2 -> ver4 -> ver3
	//
	// ver1->ver6: -> ver1 -> ver2 -> ver4 -> ver3 -> ver5 -> ver6
	//
	// ver1->ver5: -> ver2 -> ver4 -> ver3 -> ver5
	//
	// ver4->ver6: -> ver4 -> ver3 -> ver5 -> ver6
	//===================================================================

	SECTION("test runDijkstraAlgo(const string&, const string&)")
	{
		vector<VecStr> route1, route2;

		// run scene.
		grapher.runDijkstraAlgo(scene1[0], scene1[1]);
		grapher.getPathes(route1);
		REQUIRE(route1[0].size() == 6);
		REQUIRE(route1[0][0] == "ver1");
		REQUIRE(route1[0][1] == "ver2");
		REQUIRE(route1[0][2] == "ver4");
		REQUIRE(route1[0][3] == "ver3");
		REQUIRE(route1[0][4] == "ver5");
		REQUIRE(route1[0][5] == "ver6");

		// run same scene again.
		grapher.runDijkstraAlgo(scene1[0], scene1[1]);
		grapher.getPathes(route2);
		CHECK(route2.size() == 1);
		REQUIRE(route2 == route1);

		// run another scene sequentially.
		grapher.runDijkstraAlgo(scene2[0], scene2[1]);
		grapher.getPathes(route2);
		REQUIRE(route2[0].size() == 4);
		REQUIRE(route2[0][0] == "ver1");
		REQUIRE(route2[0][1] == "ver2");
		REQUIRE(route2[0][2] == "ver4");
		REQUIRE(route2[0][3] == "ver3");
	}

	SECTION("test runDijkstraAlgo(const vector<array<string, 2>>&)")
	{
		vector<VecStr> routes;

		grapher.runDijkstraAlgo(scenes);
		grapher.getPathes(routes);
		REQUIRE(routes.size() == 5);
		REQUIRE(routes[1][0] == "ver1");
		REQUIRE(routes[1][1] == "ver2");
		REQUIRE(routes[1][2] == "ver4");
		REQUIRE(routes[1][3] == "ver3");
	}

	SECTION("test runDijkstraAlgo() failure scenes")
	{
		vector<VecStr> route1, route2, routes;
		bool status = true;

		// no path between two vertices in a directed graph.
		status = grapher.runDijkstraAlgo(scene3[0], scene3[1]);
		grapher.getPathes(route2);
		CHECK(status == false);
		REQUIRE(route2.empty() == true);

		// search unknown vertice.
		status = grapher.runDijkstraAlgo(scene4[0], scene4[1]);
		grapher.getPathes(route1);
		CHECK(status == false);
		REQUIRE(route1.empty() == true);

		// one error scene in set of scenes would destroy all.
		scenes.emplace_back(scene3);
		grapher.runDijkstraAlgo(scenes);
		grapher.getPathes(routes);
		REQUIRE(routes.size() == 0);

	}

	// SECTION("test displayXXX()")
	// {
	//     bool status = true;

	//     status = grapher.runDijkstraAlgo(scenes);
	//     CHECK(status ==  true);
	//     grapher.displayGraphMatrix();
	//     grapher.displayPathes();

	//     status = grapher.runDijkstraAlgo(scene1[0], scene1[1]);
	//     CHECK(status ==  true);
	//     grapher.displayGraphMatrix();
	//     grapher.displayPathes();
	// }
}


TEST_CASE("test class DataSmoother")
{
	VecDbl orig, real, res;

	std::fstream origData("data_RiverNetAutoInit.txt", std::ios::in);
	REQUIRE(origData.is_open() == true);

	std::regex re{"[\\s,]+"};
	string line;
	while (std::getline(origData, line))
	{
		VecStr temp{std::sregex_token_iterator(line.begin(), line.end(), re, -1),
		            std::sregex_token_iterator()};
		if (temp.size() < 3 || temp[0].find("#") != string::npos)
		{
			continue;
		}

		orig.emplace_back(stod(temp[2]));
		real.emplace_back(stod(temp[1]));
	}

	std::fstream resData("result.txt", std::ios::app);
	REQUIRE(resData.is_open() == true);

	auto output = [&res, &resData](const string && item)
	{
		resData << item << "\t";
		for (const auto &val : res)
		{
			resData << val << "\t";
		}
		resData << endl << endl;
	};
	double average = std::accumulate(real.begin(), real.end(), 0.0) / real.size();
	auto standardDevitation = [&average](const VecDbl & data)
	{
		double variance = 0.0;
		for (const auto &val : data)
		{
			variance += std::pow(val - average, 2.0);
		}

		return std::sqrt(variance / data.size());
	};
	double origStandardDevitation = standardDevitation(orig);

	DataSmoother smoother;
	smoother.linearSmoothN3(orig, res);
	output("ln3");
	double sdLN3 = standardDevitation(res);
	CHECK(sdLN3 <= origStandardDevitation);

	smoother.linearSmoothN5(orig, res);
	output("ln5");
	double sdLN5 = standardDevitation(res);
	CHECK(sdLN5 <= origStandardDevitation);

	smoother.linearSmoothN7(orig, res);
	output("ln7");
	double sdLN7 = standardDevitation(res);
	CHECK(sdLN7 <= origStandardDevitation);

	smoother.quadraticSmoothN5(orig, res);
	output("qn5");
	double sdQN5 = standardDevitation(res);
	CHECK(sdQN5 <= origStandardDevitation);

	smoother.quadraticSmoothN7(orig, res);
	output("qn7");
	double sdQN7 = standardDevitation(res);
	CHECK(sdQN7 <= origStandardDevitation);

	smoother.cubicSmoothN5(orig, res);
	output("cn5");
	double sdCN5 = standardDevitation(res);
	CHECK(sdCN5 <= origStandardDevitation);

	smoother.cubicSmoothN7(orig, res);
	output("cn7");
	double sdCN7 = standardDevitation(res);
	CHECK(sdCN7 <= origStandardDevitation);
}





