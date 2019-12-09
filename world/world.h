#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <cstdlib>
#include <iostream>
#include <memory>
#include "../agent/agent.h"
#include "../agent/tft.h"
#include "../agent/bb.h"
#include "../agent/hybrid.h"
#include "results.h"

class Agent;
class Tft;

using namespace std;

class World
{
	int d_popSize;
	int d_width;
	int d_height;

	vector<shared_ptr<Agent> > d_population;

public:
	World(int mode, size_t popSize, size_t width, size_t height, float ratio1, float ratio2, float ratio3, float ratio4);
	~World(){};

	void createMode1(float ratio);
	void createMode2(float ratio);
	void createMode3(float ratio1, float ratio2, float ratio3, float ratio4);

	size_t width(){ return d_width; };
	size_t height(){ return d_height; };


	bool timeStep();
	bool isInView(Agent&);
	bool otherInView(Agent&);

	//Result reporting:
	void reportSuccessAndAgents(Results *results);

	//Evolutionary algorithm:
	double evolution(int mode);
	double evoAlg1();
	double evoAlg2();
	double evoAlg3();
	int pickParent(vector<Agent>  &survivors);
	float reproduce(float parent1, float parent2);
	//Agent fitness comparison used for sorting in the evolutionary algorithm
	bool operator() (Agent &agent1, Agent &agent2);
};

#endif
