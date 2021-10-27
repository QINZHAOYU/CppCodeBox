#include "tools/Catch/catch.hpp"
#include "common/CommConsts.hpp"
#include "modules/RiverNetAutoInit.hpp"


TEST_CASE("test class DirectedGraphHandler", "[STAGE1]")
{
        DirectedGraphHandler::Graph graph;
        graph.emplace_back(tuple<string, string, double>{"ver4", "ver5", 13});
        graph.emplace_back(tuple<string, string, double>{"ver4", "ver6", 15});        
        graph.emplace_back(tuple<string, string, double>{"ver1", "ver2", 1});
        graph.emplace_back(tuple<string, string, double>{"ver1", "ver3", 12});
        graph.emplace_back(tuple<string, string, double>{"ver2", "ver3", 9});
        graph.emplace_back(tuple<string, string, double>{"ver2", "ver4", 3});
        graph.emplace_back(tuple<string, string, double>{"ver3", "ver5", 5});
        graph.emplace_back(tuple<string, string, double>{"ver4", "ver3", 4});
        graph.emplace_back(tuple<string, string, double>{"ver5", "ver6", 4});
        
        DirectedGraphHandler grapher;
        grapher.setGraph(graph);

        SECTION("test setGraph()")
        {
            DirectedGraphHandler::GraphMatrix matrix;
            grapher.getGraphMatrix(matrix);
            REQUIRE(matrix.size() == 6);

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
}








