#include "world.h"

void World::evolution(int mode)
{
  switch (mode)
  {
    case 1:
      evoAlg1();
      break;
    case 2:
      evoAlg2();
      break;
    case 3:
      evoAlg3();
      break;
  }
}

void World::evoAlg1()
{
  vector<Agent> survivors;
  survivors.reserve(d_popSize);
  for (auto &agent : d_population)
  {
    if (agent.battery() > 0)
      survivors.push_back(agent);
  }
  sort(survivors.begin(), survivors.end(), *this);
  while (survivors.size() < d_popSize)
  {
    for (auto &agent : survivors)
    {
      if (rand()%100 < (agent.fitness() * 100))
        survivors.push_back(agent);
      if (survivors.size() == d_popSize)
        break;
    }
  }

  int i = 0;
  int selfishAgents = 0;
  int selflessAgents = 0;
  // float sum = 0;

  d_population.clear();
  d_population.reserve(d_popSize);

  for (auto &agent : survivors)
  {
    if (agent.isSelfish())
    {
      d_population.emplace_back(i, rand()%d_width, rand()%d_height, d_popSize, 1, 0);
      ++selfishAgents;
    }
    else
    {
      d_population.emplace_back(i, rand()%d_width, rand()%d_height, d_popSize, 0, 0);
      ++selflessAgents;
    }
    // d_population.emplace_back(i, rand()%d_width, rand()%d_height, d_popSize);
    // d_population[i].setSelfish(agent.isSelfish());
    ++i;
    // sum += agent.isSelfish();
  }
  // "ratio of selfish agents: " <<
  cout << (float)selfishAgents / d_population.size() << "\n";
}

void World::evoAlg2()
{
  vector<Agent> survivors;
  vector<float> children;

  survivors.reserve(d_popSize);
  for (auto &agent : d_population)
  {
    if (agent.battery() > 0)
      survivors.push_back(agent);
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
    d_population.emplace_back(i, rand()%d_width, rand()%d_height, d_popSize, survivors[i].isSelfish(), 0);
    totalSelfishness += survivors[i].isSelfish();
  }

  for (int i = 0; i < children.size(); ++i)
  {
    d_population.emplace_back(survivors.size() + i, rand()%d_width, rand()%d_height, d_popSize, children[i], 0);
    totalSelfishness += children[i];
  }
// "ratio of selfish agents: " <<
  cout << (float)totalSelfishness / d_population.size() << "\n";
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

void World::evoAlg3()
{

  vector<Agent> survivors;
  vector<float> children;
  vector<int> childrenTypes;

  survivors.reserve(d_popSize);
  for (auto &agent : d_population)
  {
    if (agent.battery() > 0)
      survivors.push_back(agent);
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
    d_population.emplace_back(i, rand()%d_width, rand()%d_height, d_popSize, survivors[i].isSelfish(), survivors[i].type());
    totalSelfishness += survivors[i].isSelfish();
  }

  for (int i = 0; i < children.size(); ++i)
  {
    d_population.emplace_back(survivors.size() + i, rand()%d_width, rand()%d_height, d_popSize, children[i], childrenTypes[i]);
    totalSelfishness += children[i];
  }

  int type0 = 0;
  int type1 = 0;
  int type2 = 0;
  int type3 = 0;
  for (Agent &agent : d_population)
  {
    switch (agent.type())
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
  // "ratio of selfish agents: " <<
  cout << (float)totalSelfishness / d_population.size();
  cout << "\t" << type0 << "\t" << type1
      << "\t" << type2 << "\t" << type3 << "\n";
  // cout << "\t" << "stochastic: " << type0 << "; TFT: " << type1
  //     << "; BB: " << type2 << "; Hybrid: " << type3 << "\n";
}
