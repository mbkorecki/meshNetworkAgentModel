#ifndef TFT_H
#define TFT_H

#include "agent.h"


class Tft : public Agent
{
	
public:
	Tft(int id, int x, int y, int populationSize, float selfishness, int type)
		: Agent(id, x, y, populationSize, selfishness, type){};
	Tft(const Tft &copy) : Agent(copy){};
	Tft(Tft &&move) : Agent(move){};
	~Tft(){};
	
	bool wantsToRoute(int agent);
};

#endif
