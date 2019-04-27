#include "world.h"

World::World(int mode, size_t popSize, size_t width, size_t height, float ratio1, float ratio2, float ratio3, float ratio4)
	: d_popSize(popSize), d_width(width), d_height(height)
{
	switch (mode)
	{
	case 1:
		createMode1(ratio1);
		break;
	case 2:
		createMode2(ratio1);
		break;
	case 3:
		createMode3(ratio1, ratio2, ratio3, ratio4);
		break;
	}
}

void World::createMode1(float ratio)
{
	d_population.reserve(d_popSize);
	for (int i = 0; i < d_popSize; ++i)
	{
		shared_ptr<Agent> agent(new Agent{i, rand()%d_width, rand()%d_height, d_popSize, 0, 0});
		d_population.emplace_back(agent);
	}
	for (int i = 0; i < d_popSize * ratio; ++i)
	{
		d_population[i].get()->setSelfish(1);
	}

	float sum = 0;
	for (auto &entry : d_population)
		sum += entry.get()->isSelfish();
}

void World::createMode2(float ratio)
{
	d_population.reserve(d_popSize);
	float selfishness;
	float totalSelfishness = 0;

	for (int i = 0; i < d_popSize; ++i)
	{
		selfishness = (rand()%100) / 100.0;
		totalSelfishness += selfishness;
		shared_ptr<Agent> agent(new Agent{i, rand()%d_width, rand()%d_height, d_popSize, selfishness, 0});
		d_population.emplace_back(agent);
	}
	int i = 0;
	while ((totalSelfishness / d_popSize > ratio) && i < d_popSize)
	{
		totalSelfishness -= d_population[i].get()->isSelfish();
		d_population[i].get()->setSelfish(0);
		++i;
	}
	i = 0;
	while ((totalSelfishness / d_popSize < ratio) && i < d_popSize)
	{
		totalSelfishness = totalSelfishness - d_population[i].get()->isSelfish() + 1;
		d_population[i].get()->setSelfish(1);
		++i;
	}
	float sum = 0;
	for (auto &entry : d_population)
		sum += entry.get()->isSelfish();
}

void World::createMode3(float ratioOfSelfish, float ratioOfTft, float ratioOfBb, float ratioOfHybrid)
{
	d_population.reserve(d_popSize);

	float ratioOfStochastic = 1 - ratioOfTft  - ratioOfBb - ratioOfHybrid;
	
	float totalSelfishness = 0;
	float selfishness;

	int currentStep = d_popSize * ratioOfStochastic;

	for (int i = 0; i < currentStep; ++i)
	{
		selfishness = (rand()%100) / 100.0;
		totalSelfishness += selfishness;
		shared_ptr<Agent> agent(new Agent{i, rand()%d_width, rand()%d_height, d_popSize, selfishness, 0});
		d_population.emplace_back(agent);
	}
	for (int i = currentStep; i < static_cast<int>(currentStep + d_popSize * ratioOfTft); ++i)
	{
		shared_ptr<Tft> agent(new Tft{i, rand()%d_width, rand()%d_height, d_popSize, 0, 1});
		d_population.emplace_back(agent);
	}
	currentStep += d_popSize * ratioOfTft;

	for (int i = currentStep; i < static_cast<int>(currentStep + d_popSize * ratioOfBb); ++i)
	{
		shared_ptr<Bb> agent(new Bb{i, rand()%d_width, rand()%d_height, d_popSize, 0, 2});
		d_population.emplace_back(agent);
	}
	currentStep += d_popSize * ratioOfBb;

	for (int i = currentStep; i < static_cast<int>(currentStep + d_popSize * ratioOfHybrid); ++i)
	{
		shared_ptr<Hybrid> agent(new Hybrid{i, rand()%d_width, rand()%d_height, d_popSize, 0, 3});
		d_population.emplace_back(agent);
	}

	int i = d_popSize * ratioOfStochastic;
	while ((totalSelfishness / d_popSize > ratioOfSelfish) && i >= 0)
	{
		totalSelfishness -= d_population[i].get()->isSelfish();
		d_population[i].get()->setSelfish(0);
		--i;
	}
	i = 0;
	while ((totalSelfishness / d_popSize < ratioOfSelfish) && i < d_popSize * ratioOfStochastic)
	{
		totalSelfishness = totalSelfishness - d_population[i].get()->isSelfish() + 1;
		d_population[i].get()->setSelfish(1);
		++i;
	}
	float sum = 0;
	for (auto &entry : d_population)
	{
		sum += entry.get()->isSelfish();
	}
}


bool World::timeStep()
{
	int dead = 0;
	for (auto &agent : d_population)
	{
		agent.get()->setWillingnessToRoute();
		if (agent.get()->battery() <= 0)
		{
			dead++;
			agent.get()->die();
			if (dead >= d_popSize / 2)
				return false;
		}
		else
			agent.get()->move(isInView(*agent.get()), *this);
	}
	for (auto &agent : d_population)
	{
		if (isInView(*agent.get()))
		{
			agent.get()->updateRouteTable(d_population, *this);
		}
		else
		{
			agent.get()->flushRouteTable();
		}
	}
	bool improved = true;
	while (improved)
	{
		improved = false;
		for (auto &agent : d_population)
		{
			if (isInView(*agent.get()) && agent.get()->isTableUpdated())
			{
				if (agent.get()->advertiseTable(d_population, *this))
					improved = true;
			}
		}
	}
	for (auto &agent : d_population)
	{
		if (isInView(*agent.get()) && agent.get()->wantsToSend() && otherInView(*agent.get()))
		{
			int recieverID = rand() % d_popSize;
			while (recieverID == agent.get()->id() || !isInView(*d_population[recieverID].get()))
				recieverID = rand() % d_popSize;
			agent.get()->sendMessage(*d_population[recieverID].get(), d_population);
		}
	}
	return true;
}

bool World::isInView(Agent &agent)
{
	if (agent.x() >= 0 && agent.x() < d_width && agent.y() >= 0 && agent.y() < d_height)
		return true;
	else
		return false;
}

bool World::otherInView(Agent &agent)
{
	for (auto &a : d_population)
		if (a.get()->id() != agent.id() && isInView(*a.get()))
			return true;
	return false;
}

bool World::operator()(Agent &agent1, Agent &agent2)
{
	return agent1.fitness() > agent2.fitness();
}

void World::reportSuccessAndAgents(Results *results)
{
	int msgSum = 0;
	int msgRtd = 0;
	int msgRec = 0;
	double type0 = 0;
	double type1 = 0;
	double type2 = 0;
	double type3 = 0;
	
	float averageSuccess = 0;
	for (auto &agent : d_population)
	{
		averageSuccess += agent.get()->fitness();

		msgSum += agent.get()->messagesTotal();
		msgRtd += agent.get()->messagesRouted();
		msgRec += agent.get()->messagesRecieved();
		
		switch (agent.get()->type())
		{
		case 0:
			++type0;
			break;
		case 1:
			++type1;
			break;
		case 2:
			++type2;
			break;
		case 3:
			++type3;
			break;
		}
	}
	averageSuccess /= d_popSize;
	results->numStochastic = type0;
	results->numTFT = type1;
	results->numBB = type2;
	results->numHybrid = type3;
	results->effectivness = averageSuccess;
}
