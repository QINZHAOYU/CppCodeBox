#include "tools/Catch/catch.hpp"
#include "common/CommConsts.hpp"
#include "modules/RiverNetAutoInit.hpp"


TEST_CASE("test class DirectedGraphHandler", "[STAGE1]")
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

    vector<array<string, 2>> scenes = {
        {"ver1", "ver2"},
        {"ver1", "ver3"},
        {"ver1", "ver6"},
        {"ver2", "ver5"},
        {"ver4", "ver6"},};  
    array<string, 2> scene1 =  {"ver1", "ver6"}; 
    array<string, 2> scene2 =  {"ver1", "ver3"};
    array<string, 2> scene3 =  {"ver3", "ver1"};

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
        VecStr route1, route2;
        array<string, 2> currVers;

        // run scene.
        grapher.runDijkstraAlgo(scene1[0], scene1[1]);
        grapher.getCurrPath(route1);
        REQUIRE(route1.size() == 6);
        REQUIRE(route1[0] == "ver1");
        REQUIRE(route1[1] == "ver2");        
        REQUIRE(route1[2] == "ver4");
        REQUIRE(route1[3] == "ver3");
        REQUIRE(route1[4] == "ver5");        
        REQUIRE(route1[5] == "ver6");

        // run same scene again.
        grapher.runDijkstraAlgo(scene1[0], scene1[1]);
        grapher.getCurrPath(route2);
        REQUIRE(route1 == route2);

        // run another scene sequentially.
        grapher.runDijkstraAlgo(scene2[0], scene2[1]);
        grapher.getCurrPath(route2);
        REQUIRE(route2.size() == 4);
        REQUIRE(route1[0] == "ver1");
        REQUIRE(route1[1] == "ver2");        
        REQUIRE(route1[2] == "ver4");
        REQUIRE(route1[3] == "ver3");

        // run an scene without solution in a directed grapher.
        bool status = grapher.runDijkstraAlgo(scene3[0], scene3[1]);   
        grapher.getCurrPath(route2);
        grapher.getCurrVertices(currVers);
        CHECK(status == false);        
        REQUIRE(currVers != scene3);
        REQUIRE(route2.empty() == true);
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








