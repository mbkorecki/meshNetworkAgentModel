#include "world.h"
#include "results.h"

double World::evolution(int mode)
{
	switch (mode)
	{
	case 1:
		return evoAlg1();
	case 2:
		return evoAlg2();
	case 3:
		return evoAlg3();
	}
	return 0;
}

double World::evoAlg1()
{
	vector<Agent> survivors;
	survivors.reserve(d_popSize);
	for (auto &agent : d_population)
	{
		if (agent.get()->battery() > 0)
			survivors.push_back(*agent.get());
	}
	sort(survivors.begin(), survivors.end(), *this);
	while (survivors.size() < d_popSize)
	{
		for (auto &agent : survivors)
		{
			if (rand()%100 <= (agent.fitness() * 100))
				survivors.push_back(agent);
			if (survivors.size() == d_popSize)
				break;
		}
	}

	int i = 0;
	int selfishAgents = 0;
	int selflessAgents = 0;

	d_population.clear();
	d_population.reserve(d_popSize);

	for (auto &agent : survivors)
	{
		if (agent.isSelfish())
		{
			shared_ptr<Agent> agent(new Agent{i, rand()%d_width, rand()%d_height, d_popSize, 1, 0});
			d_population.emplace_back(agent);
			++selfishAgents;
		}
		else
		{
			shared_ptr<Agent> agent(new Agent{i, rand()%d_width, rand()%d_height, d_popSize, 0, 0});
			d_population.emplace_back(agent);
			++selflessAgents;
		}
		++i;
	}
	return (float)selfishAgents / d_population.size();
}

double World::evoAlg2()
{
	vector<Agent> survivors;
	vector<float> children;

	survivors.reserve(d_popSize);
	for (auto &agent : d_population)
	{
		if (agent.get()->battery() > 0)
			survivors.push_back(*agent.get());
	}
	sort(survivors.begin(), survivors.end(), *this);

	while (survivors.size() + children.size() < d_popSize)
	{
		int parent1 = pickParent(survivors);
		int parent2 = pickParent(survivors);
		float child = reproduce(survivors[parent1].isSelfish(), survivors[parent2].isSelfish());

		if (rand()%100 == 0) //mutation
		{
			if (rand()%1 == 0)
				child += 0.1;
			else
				child -= 0.1;
		}
		children.push_back(child);
	}

	d_population.clear();
	d_population.reserve(d_popSize);
	float totalSelfishness = 0;

	for (int i = 0; i < survivors.size(); ++i)
	{
		shared_ptr<Agent> agent(new Agent{i, rand()%d_width, rand()%d_height, d_popSize, survivors[i].isSelfish(), 0});
		d_population.emplace_back(agent);
		totalSelfishness += survivors[i].isSelfish();
	}

	for (int i = 0; i < children.size(); ++i)
	{
		shared_ptr<Agent> agent(new Agent{static_cast<int>(survivors.size() + i), rand()%d_width, rand()%d_height, d_popSize, children[i], 0});
		d_population.emplace_back(agent);
		totalSelfishness += children[i];
	}
// "ratio of selfish agents: " <<
	return (float)totalSelfishness / d_population.size();
}

int World::pickParent(vector<Agent> &survivors)
{
	int startingPoint = rand()%survivors.size();
	while (startingPoint >= 0)
	{
		if (survivors[startingPoint].fitness() > ((rand()%100) / 100.0))
			return startingPoint;
		--startingPoint;
	}
	return 0; //returns most fit individual by default
}

float World::reproduce(float parent1, float parent2)
{
	return (parent1 + parent2) / 2.0;
}

double World::evoAlg3()
{

	vector<Agent> survivors;
	vector<float> children;
	vector<int> childrenTypes;

	survivors.reserve(d_popSize);
	for (auto &agent : d_population)
	{
		if (agent.get()->battery() > 0)
			survivors.push_back(*agent.get());
	}
	sort(survivors.begin(), survivors.end(), *this);

	while (survivors.size() + children.size() < d_popSize)
	{
		int parent1 = pickParent(survivors);
		int parent2 = pickParent(survivors);
		float child = reproduce(survivors[parent1].isSelfish(), survivors[parent2].isSelfish());
		int childType = (rand()%1 ? survivors[parent1].type() : survivors[parent2].type());
		if (childType != 0)
			child = 0;

		if (rand()%100 == 0) //mutation
		{
			if (rand()%1 == 0)
				child += 0.1;
			else
				child -= 0.1;
		}
		children.push_back(child);
		childrenTypes.push_back(childType);
	}

	d_population.clear();
	d_population.reserve(d_popSize);
	float totalSelfishness = 0;

	for (int i = 0; i < survivors.size(); ++i)
	{
		switch (survivors[i].type())
		{
		case 0:
		{
			shared_ptr<Agent> agent(new Agent{i, rand()%d_width, rand()%d_height, d_popSize, survivors[i].isSelfish(), survivors[i].type()});
			d_population.emplace_back(agent);
		}
		break;
		case 1:
		{
			shared_ptr<Tft> agent(new Tft{i, rand()%d_width, rand()%d_height, d_popSize, survivors[i].isSelfish(), survivors[i].type()});
			d_population.emplace_back(agent);
		}
		break;
		case 2:
		{
			shared_ptr<Bb> agent(new Bb{i, rand()%d_width, rand()%d_height, d_popSize, survivors[i].isSelfish(), survivors[i].type()});
			d_population.emplace_back(agent);
		}
		break;
		case 3:
		{
			shared_ptr<Hybrid> agent(new Hybrid{i, rand()%d_width, rand()%d_height, d_popSize, survivors[i].isSelfish(), survivors[i].type()});
			d_population.emplace_back(agent);
		}
		break;
		}
		totalSelfishness += survivors[i].isSelfish();
	}

	for (int i = 0; i < children.size(); ++i)
	{
		switch (childrenTypes[i])
		{
		case 0:
		{
			shared_ptr<Agent> agent(new Agent{static_cast<int>(survivors.size() + i), rand()%d_width, rand()%d_height, d_popSize, children[i], childrenTypes[i]});
			d_population.emplace_back(agent);
		}
		break;
		case 1:
		{
			shared_ptr<Tft> agent(new Tft{static_cast<int>(survivors.size() + i), rand()%d_width, rand()%d_height, d_popSize, children[i], childrenTypes[i]});
			d_population.emplace_back(agent);
		}
		break;
		case 2:
		{
			shared_ptr<Bb> agent(new Bb{static_cast<int>(survivors.size() + i), rand()%d_width, rand()%d_height, d_popSize, children[i], childrenTypes[i]});
			d_population.emplace_back(agent);
		}
		break;
		case 3:
		{
			shared_ptr<Hybrid> agent(new Hybrid{static_cast<int>(survivors.size() + i), rand()%d_width, rand()%d_height, d_popSize, children[i], childrenTypes[i]});
			d_population.emplace_back(agent);
		}
		break;
		}
		totalSelfishness += children[i];
	}

	return static_cast<double>(totalSelfishness) / d_population.size();
}
