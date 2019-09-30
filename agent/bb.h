#ifndef BB_H
#define BB_H

#include "agent.h"


class Bb : public Agent
{

public:
	Bb(int id, int x, int y, int populationSize, float selfishness, int type)
		: Agent(id, x, y, populationSize, selfishness, type){};
	Bb(const Bb &copy) : Agent(copy){};
	Bb(Bb &&move) : Agent(move){};
	~Bb(){};

	void setWillingnessToRoute();
};

#endif
