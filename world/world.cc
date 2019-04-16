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
    d_population.emplace_back(i, rand()%d_width, rand()%d_height, d_popSize, 0, 0);
  }
  for (int i = 0; i < d_popSize * ratio; ++i)
  {
    d_population[i].setSelfish(1);
  }
  float sum = 0;
  for (auto &entry : d_population)
    sum += entry.isSelfish();
  cout << "lvl of selfishness: " << sum / d_population.size() << "\n";
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
    d_population.emplace_back(i, rand()%d_width, rand()%d_height, d_popSize, selfishness, 0);
  }
  int i = 0;
  while ((totalSelfishness / d_popSize > ratio) && i < d_popSize)
  {
    totalSelfishness -= d_population[i].isSelfish();
    d_population[i].setSelfish(0);
    ++i;
  }
  i = 0;
  while ((totalSelfishness / d_popSize < ratio) && i < d_popSize)
  {
    totalSelfishness = totalSelfishness - d_population[i].isSelfish() + 1;
    d_population[i].setSelfish(1);
    ++i;
  }
  float sum = 0;
  for (auto &entry : d_population)
    sum += entry.isSelfish();
  cout << "lvl of selfishness: " << sum / d_population.size() << "\n";
}

void World::createMode3(float ratioOfSelfish, float ratioOfTft, float ratioOfBb, float ratioOfHybrid)
{
  d_population.reserve(d_popSize);

  float ratioOfSelfless = 1 - ratioOfSelfish - ratioOfTft  - ratioOfBb - ratioOfHybrid;

  float totalSelfishness = 0;
  float selfishness;

  int currentStep = d_popSize * (ratioOfSelfless + ratioOfSelfish);

  for (int i = 0; i < currentStep; ++i)
  {
    selfishness = (rand()%100) / 100.0;
    totalSelfishness += selfishness;
    d_population.emplace_back(i, rand()%d_width, rand()%d_height, d_popSize, selfishness, 0);
  }

  for (int i = currentStep; i < int(currentStep + d_popSize * ratioOfTft); ++i)
  {
    d_population.emplace_back(i, rand()%d_width, rand()%d_height, d_popSize, 0, 1);
  }
  currentStep += d_popSize * ratioOfTft;

  for (int i = currentStep; i < int(currentStep + d_popSize * ratioOfBb); ++i)
  {
    d_population.emplace_back(i, rand()%d_width, rand()%d_height, d_popSize, 0, 2);
  }
  currentStep += d_popSize * ratioOfBb;

  for (int i = currentStep; i < int(currentStep + d_popSize * ratioOfHybrid); ++i)
  {
    d_population.emplace_back(i, rand()%d_width, rand()%d_height, d_popSize, 0, 3);
  }

  int i = 0;
  while ((totalSelfishness / d_popSize > ratioOfSelfish) && i < d_popSize * (ratioOfSelfless + ratioOfSelfish))
  {
    totalSelfishness -= d_population[i].isSelfish();
    d_population[i].setSelfish(0);
    ++i;
  }
  i = 0;
  while ((totalSelfishness / d_popSize < ratioOfSelfish) && i < d_popSize * (ratioOfSelfless + ratioOfSelfish))
  {
    totalSelfishness = totalSelfishness - d_population[i].isSelfish() + 1;
    d_population[i].setSelfish(1);
    ++i;
  }
  float sum = 0;
  for (auto &entry : d_population)
  {
    sum += entry.isSelfish();
  }
  cout << "lvl of selfishness: " << sum / d_population.size() << "\n";
}


bool World::timeStep()
{
  int dead = 0;
  for (Agent &agent : d_population)
  {
    agent.setWillingnessToRoute();
    if (agent.battery() <= 0)
    {
      dead++;
      agent.die();
      if (dead == d_popSize / 2)
        return false;
    }
    else
      agent.move(isInView(agent), *this);
  }
  for (Agent &agent : d_population)
  {
    if (isInView(agent))
    {
      agent.updateRouteTable(d_population, *this);
    }
    else
    {
      agent.flushRouteTable();
    }
  }
  bool improved = true;
  while (improved)
  {
    improved = false;
    for (Agent &agent : d_population)
    {
      if (isInView(agent) && agent.isTableUpdated())
      {
        if (agent.advertiseTable(d_population, *this))
          improved = true;
      }
    }
  }
  for (Agent &agent : d_population)
  {
    if (isInView(agent) && agent.wantsToSend() && otherInView(agent))
    {
      int recieverID = rand() % d_popSize;
      while (recieverID == agent.id() || !isInView(d_population[recieverID]))
        recieverID = rand() % d_popSize;
      agent.sendMessage(d_population[recieverID], d_population);
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
    if (a.id() != agent.id() && isInView(a))
      return true;
  return false;
}

bool World::operator()(Agent &agent1, Agent &agent2)
{
  return agent1.fitness() > agent2.fitness();
}


void World::report()
{
  int msgSum = 0;
  int msgRtd = 0;
  int msgRec = 0;
  float averageSuccess = 0;
  for (Agent &agent : d_population)
  {
    averageSuccess += agent.fitness();
    // cout << "agent " << agent.id() << " msg total: " << agent.messagesTotal()
    //      << ", msg sent: " << agent.messagesSent() << ", ratio: "
    //      << (float)agent.messagesSent() / agent.messagesTotal() << "\n"
    //      << ", msg routed: " << agent.messagesRouted()
    //      << ", msg recieved: " << agent.messagesRecieved()
    //      << ", battery: " << agent.battery() << "\n";
    msgSum += agent.messagesTotal();
    msgRtd += agent.messagesRouted();
    msgRec += agent.messagesRecieved();
  }
  averageSuccess /= d_popSize;
  // cout << "\tsum of messages: " << msgSum << "\n";
  // cout << "\tsum of recieved messages: " << msgRec << "\n";
  // cout << "\tsum of routed messages: " << msgRtd << "\n";
  // cout << "\taverage success/number of epochs/selfish ratio\n";
  // cout << "-------------------------------\n";
  cout << std::setprecision(3);
  cout << averageSuccess << "\t";
}
