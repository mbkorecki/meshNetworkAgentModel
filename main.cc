#include <cstdlib>
#include <iostream>
#include <ctime>
#include <sstream>

#include "world/world.h"

using namespace std;

void prompt()
{
  cout << "Not enough arguments. \nUsage: ./app x a b c d e (f g h), where: \n"
      << "x = the mode in which to run the simulation, 1 - binary, 2 - stochastic, 3 - advanced strategies \n"
      << "a = the initial population \n"
      << "b, c = the dimensions of the world \n"
      << "d = the number of epochs the simulation will be run for \n"
      << "e = the initial ratio of selfish agents in the population \n"
      << "f = the initial ratio of tit for tat agents (used only for mode 3) \n"
      << "g = the initial ratio of battery based agents (used only for mode 3) \n"
      << "h = the initial ratio of hybrid agents (used only for mode 3) \n";
}

int main(int argc, char *argv[])
{
  float ratioOfTft = 0;
  float ratioOfBb = 0;
  float ratioOfHybrid = 0;

  if (argc < 7) //not enough arguments
  {
    prompt();
    return 0;
  }

  std::istringstream ss1(argv[1]);
  int mode;
  ss1 >> mode;

  if (mode == 3)
  {
    if (argc < 10)
    {
      prompt();
      return 0;
    }
    else
    {
      std::istringstream ss7(argv[7]);
      ss7 >> ratioOfTft;
      std::istringstream ss8(argv[8]);
      ss8 >> ratioOfBb;
      std::istringstream ss9(argv[9]);
      ss9 >> ratioOfHybrid;
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
	float ratioOfSelfish;
	ss6 >> ratioOfSelfish;


	srand(time(0));
	World world{mode, population, width, height, ratioOfSelfish, ratioOfTft, ratioOfBb, ratioOfHybrid};
  cout << "----------------------------------------------------------\n";
  cout << "effect\tepochs\tselfish\tstoch\tTTF\tBB\tHybrid\n";
	for (int i = 0; i < maxEras; ++i)
	{
    // cout << "------------------------------\n";
		int j = 0;
		while (world.timeStep())
		{
			j++;
		}
    // "half the population is dead in epoch " <<
    world.report();
    cout <<  j << "\t";

		world.evolution(mode);
	}
}
