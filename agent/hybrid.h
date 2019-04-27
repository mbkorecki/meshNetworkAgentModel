#ifndef HYBRID_H
#define HYBRID_H

#include "agent.h"


class Hybrid : public Agent
{
	
public:
	Hybrid(int id, int x, int y, int populationSize, float selfishness, int type)
		: Agent(id, x, y, populationSize, selfishness, type){};
	Hybrid(const Hybrid &copy) : Agent(copy){};
	Hybrid(Hybrid &&move) : Agent(move){};
	~Hybrid(){};
	
	bool wantsToRoute(int agent);
	void setWillingnessToRoute();
};

#endif
