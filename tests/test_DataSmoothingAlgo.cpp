#include "tools/Catch/catch.hpp"
#include "common/CommConsts.hpp"

#include "modules/DataSmoothingAlgo.hpp"
#include <fstream>
#include <regex>
#include <numeric>

using namespace ccb;


TEST_CASE("test class DataSmoother")
{
	VecDbl orig, real, res;

	std::fstream origData("data_DataSmoothingAlgo.txt", std::ios::in);
	REQUIRE(origData.is_open() == true);

	std::regex re{"[\\s,]+"};  // regex spliting delims.
	string line;
	while (std::getline(origData, line))
	{
		// regex split string.
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


