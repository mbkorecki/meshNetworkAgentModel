#include <cstdlib>
#include <iostream>
#include <ctime>
#include <sstream>

#include "world/world.h"
#include "world/results.h"

using namespace std;

void prompt()
{
	cout << "Not enough arguments. \n\nUsage: ./app x a b c d e f (g h i)\n  where\n"
	     << "    x = set-up to use: 1 - binary, 2 - stochastic, 3 - advanced strategies\n"
	     << "    a = number of agents\n"
	     << "    b c = size of the world\n"
	     << "    d = number of days to simulate\n"
	     << "    e = how often the simulation should run\n"
	     << "    f = initial ratio of selfish agents\n"
	     << "    g = initial ratio of tit-for-tat agents   (used only for set-up 3)\n"
	     << "    h = initial ratio of battery-based agents (used only for set-up 3)\n"
	     << "    i = initial ratio of hybrid agents        (used only for set-up 3)\n\n";
}


int main(int argc, char *argv[])
{
	float ratioOfTft = 0;
	float ratioOfBb = 0;
	float ratioOfHybrid = 0;

	if (argc < 8) //not enough arguments
	{
		prompt();
		return 0;
	}

	std::istringstream ss1(argv[1]);
	int mode;
	ss1 >> mode;

	if (mode == 3)
	{
		if (argc < 11)
		{
			prompt();
			return 0;
		}
		else
		{
			std::istringstream ss8(argv[8]);
			ss8 >> ratioOfTft;
			std::istringstream ss9(argv[9]);
			ss9 >> ratioOfBb;
			std::istringstream ss10(argv[10]);
			ss10 >> ratioOfHybrid;
		}
	}


	std::istringstream ss2(argv[2]);
	size_t population;
	ss2 >> population;
	std::istringstream ss3(argv[3]);
	size_t width;
	ss3 >> width;
	std::istringstream ss4(argv[4]);
	size_t height;
	ss4 >> height;
	std::istringstream ss5(argv[5]);
	size_t maxEras;
	ss5 >> maxEras;
	std::istringstream ss6(argv[6]);
	size_t dataPoints;
	ss6 >> dataPoints;
	std::istringstream ss7(argv[7]);
	float ratioOfSelfish;
	ss7 >> ratioOfSelfish;
   	
	srand(time(0));
	cout << "day\truns\teffect\teffectSD\tselfish\tselfishSD\tstoch\tTTF\tBB\tHybrid\n";

	Results **results = new Results*[dataPoints];
	for (int i = 0; i < dataPoints; ++i)
		results[i] = new Results[maxEras];
	
	for (int k = 0; k < dataPoints; ++k)
	{
		World world{mode, population, width, height, ratioOfSelfish, ratioOfTft, ratioOfBb, ratioOfHybrid};
		for (int i = 0; i < maxEras; ++i)
		{
			int j = 0;
			while (world.timeStep())
			{
				j++;
			}
			// "half the population is dead in epoch " <<
			world.reportSuccessAndAgents(&results[k][i]);
			results[k][i].epochs = j;
			results[k][i].selfishness = world.evolution(mode);
		}	
	}

	Results *averageResult = new Results[maxEras];
	for (int i = 0; i < maxEras; ++i)
	{
		for (int k = 0; k < dataPoints; ++k)
		{
			averageResult[i].effectivness += results[k][i].effectivness;
			averageResult[i].epochs += results[k][i].epochs;
			averageResult[i].selfishness += results[k][i].selfishness;
			averageResult[i].numStochastic += results[k][i].numStochastic;
			averageResult[i].numTFT += results[k][i].numTFT;
			averageResult[i].numBB += results[k][i].numBB;
			averageResult[i].numHybrid += results[k][i].numHybrid;
		}
		averageResult[i].effectivness /= dataPoints;
		averageResult[i].epochs /= dataPoints;
		averageResult[i].selfishness /= dataPoints;
		averageResult[i].numStochastic /= dataPoints;
		averageResult[i].numTFT /= dataPoints;
		averageResult[i].numBB /= dataPoints;
		averageResult[i].numHybrid /= dataPoints;
	}

	for (int i = 0; i < maxEras; ++i)
	{
		for (int k = 0; k < dataPoints; ++k)
		{
			double term = (results[k][i].selfishness - averageResult[i].selfishness);
			averageResult[i].selfishSD += term * term;
			term = (results[k][i].effectivness - averageResult[i].effectivness);
			averageResult[i].effectSD += term * term;
		}
		averageResult[i].selfishSD /= dataPoints;
		averageResult[i].effectSD /= dataPoints;
	}
	
	
	for (int i = 0; i < maxEras; ++i) {
		cout << (i + 1) << "\t"; // print the day in the first column
		averageResult[i].print();
	}

	for (int i = 0; i < dataPoints; ++i)
		delete[] results[i];
	delete[] results;
	delete[] averageResult;
	
	return 0;
}
