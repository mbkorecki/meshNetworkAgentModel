#include "results.h"
#include <iostream>
#include <iomanip>

void Results::print()
{
	std::cout << std::fixed;
	std::cout << std::setprecision(3);
	std::cout << epochs << "\t"
	          << effectivness << "\t"
	          << effectSD << "\t\t"
	          << selfishness << "\t"
	          << selfishSD << "\t\t"
	          << numStochastic << "\t"
	          << numTFT << "\t"
	          << numBB << "\t"
	          << numHybrid << "\n";
}
