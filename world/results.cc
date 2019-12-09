#include "results.h"
#include <iostream>
#include <iomanip>

void Results::print()
{
	const std::string divider = "\t";
	std::cout << std::fixed;
	std::cout << std::setprecision(3);
	std::cout << epochs        << divider
		  << effectivness  << divider
		  << effectSD      << divider
		  << selfishness   << divider
		  << selfishSD     << divider
		  << numStochastic << divider
		  << numTFT        << divider
		  << numBB         << divider
		  << numHybrid << " \n";
}
