#ifndef RESULTS_H
#define RESULTS_H


struct Results
{
	double selfishness = 0.0;
	double effectivness = 0.0;
	double selfishSD = 0.0;
	double effectSD = 0.0;
	double epochs = 0.0;
	double numStochastic = 0.0;
	double numTFT = 0.0;
	double numBB = 0.0;
	double numHybrid = 0.0;

	void print();
};


#endif
