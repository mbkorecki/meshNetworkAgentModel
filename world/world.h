#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include "../agent/agent.h"

class Agent;

using namespace std;

class World
{
  int d_popSize;
  int d_width;
  int d_height;

  vector<Agent> d_population;

public:
  World(int mode, size_t popSize, size_t width, size_t height, float ratio1, float ratio2, float ratio3, float ratio4);
  // World(const World&){};
  // World(World&&){};
  ~World(){};

  void createMode1(float ratio);
  void createMode2(float ratio);
  void createMode3(float ratio1, float ratio2, float ratio3, float ratio4);

  bool timeStep();

  void report();

  size_t width(){ return d_width; };
  size_t height(){ return d_height; };

  bool isInView(Agent&);
  bool otherInView(Agent&);

  void evolution(int mode);
  void evoAlg1();
  void evoAlg2();
  void evoAlg3();
  int pickParent(vector<Agent> &survivors);
  float reproduce(float parent1, float parent2);

  bool operator() (Agent &agent1, Agent &agent2);
};

#endif
