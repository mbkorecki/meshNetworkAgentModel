#include "agent.h"
#include "../world/world.h"


Agent::Agent(int id, int x, int y, int popSize, float selfishness, int type)
	: d_id(id), d_x(x), d_y(y), d_battery(1000), d_selfish(selfishness), d_type(type)
{
	d_memoryTable.reserve(popSize);
	d_routeTable.reserve(popSize);
	for (int i = 0; i < popSize; ++i)
	{
		d_routeTable.emplace_back(-1, 99999999);
		int val = 1;
		d_memoryTable.push_back(val);
	}
}

Agent::Agent(const Agent &copy)
	: d_routeTable(copy.d_routeTable), d_memoryTable(copy.d_memoryTable), d_type(copy.d_type),
	  d_id(copy.d_id), d_x(copy.d_x),
	  d_y(copy.d_y), d_battery(copy.d_battery), d_selfish(copy.d_selfish),
	  d_messagesSent(copy.d_messagesSent), d_messagesTotal(copy.d_messagesTotal),
	  d_messagesRouted(copy.d_messagesRouted), d_messagesRecieved(copy.d_messagesRecieved)
{
}

Agent::Agent(Agent &&move)
	: d_routeTable(move.d_routeTable), d_memoryTable(move.d_memoryTable), d_type(move.d_type),
	  d_id(move.d_id), d_x(move.d_x),
	  d_y(move.d_y), d_battery(move.d_battery), d_selfish(move.d_selfish),
	  d_messagesSent(move.d_messagesSent), d_messagesTotal(move.d_messagesTotal),
	  d_messagesRouted(move.d_messagesRouted), d_messagesRecieved(move.d_messagesRecieved)
{
}

void Agent::move(bool mode, World &world)
{
	int direction = rand() % 5;
	if (mode)
	{
		switch (direction)
		{
		case 0:
			break;
		case 1:
			++d_x;
			break;
		case 2:
			--d_x;
			break;
		case 3:
			++d_y;
			break;
		case 4:
			--d_y;
			break;
		}
	}
	else
	{
		if ((rand() % 100) < d_return * 10)
		{
			d_return = 1;
			int sideOfReentry = rand() % 4;
			switch (sideOfReentry)
			{
			case 0:
				d_x = rand() % world.width();
				d_y = 0;
				break;
			case 1:
				d_x = rand() % world.width();
				d_y = world.height();
				break;
			case 2:
				d_x = world.width();
				d_y = rand() % world.height();
				break;
			case 3:
				d_x = rand() % world.height();
				d_y = rand() % world.height();
				break;
			}
		}
		else
		{
			d_return++;
		}
	}
}

void Agent::setWillingnessToRoute()
{	
	if (rand()%100 > (d_selfish * 100))
		d_wantsToRoute = true;
	else
		d_wantsToRoute = false;

}

bool Agent::wantsToSend()
{
	if (rand() % 4 == 0)
	{
		return true;
	}
	return false;
	//25% chance that an agent wants to send a message
}


bool Agent::isWithinRange(Agent &agent, World &world)
{
	int radius = 10;
	if (!world.isInView(agent))
		return false;
	if ((pow((agent.x() - d_x), 2) + pow((agent.y() - d_y), 2)) < pow(radius, 2))
		return true;
	else
		return false;
}

bool Agent::isNeighbour(Agent &agent, World &world)
{
	if (isWithinRange(agent, world) && agent.id() != this->id() && isWithinRange(*this, world))
		return true;
	else
		return false;
}

float Agent::distanceTo(Agent &agent)
{
	return sqrt(pow((agent.x() - d_x), 2) + pow((agent.y() - d_y), 2));
}


void Agent::flushRouteTable()
{
	for (auto &entry : d_routeTable)
	{
		entry.nextHop = -1;
		entry.hopCount = 99999999;
	}
}

void Agent::updateRouteTable(vector<shared_ptr<Agent> > &population, World &world)
{
	flushRouteTable();

	for (auto &agent : population)
	{
		if (isNeighbour(*agent.get(), world))
		{
			d_routeTable[agent.get()->id()] = TableEntry{agent.get()->id(), 1};
		}
		else
		{
			d_routeTable[agent.get()->id()] = TableEntry{-1, 99999999};
		}
	}
	d_routeTable[this->id()] = TableEntry{this->id(), 0};
}

bool Agent::advertiseTable(vector<shared_ptr<Agent> > &population, World &world)
{
	bool improved = false;
	for (auto &agent : population)
	{
		if (isNeighbour(*agent.get(), world))
		{
			if (agent.get()->recieveTable(d_routeTable, this->id(), population))
			{
				improved = true;
				agent.get()->setTableTo(true);
			}
		}
	}
	return improved;
}

bool Agent::recieveTable(vector<TableEntry> &table, int senderID, vector<shared_ptr<Agent> > &population)
{
	int idx = 0;
	bool improved = false;
	// if (!population[senderID].get()->wantsToRoute(this->id()))
	// { 
	// 	if (d_type != 0)
	// 	{
	// 		d_memoryTable[senderID] = 0;
	// 	}
	// 	if (d_routeTable[senderID].hopCount >= (table[senderID].hopCount + 1))
	// 	{
	// 		if (d_routeTable[senderID].hopCount == (table[senderID].hopCount + 1))
	// 		{
	// 			if ((rand()%population.size()) > 0.7 * population.size())
	// 			{
	// 				d_routeTable[senderID].nextHop = senderID;
	// 				d_routeTable[senderID].hopCount = (table[senderID].hopCount + 1);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			improved = true;
	// 			d_routeTable[senderID].nextHop = senderID;
	// 			d_routeTable[senderID].hopCount = (table[senderID].hopCount + 1);
	// 		}
	// 	}
	// }
	//else
	{
		for (TableEntry &entry : table)
		{
			if (d_routeTable[idx].hopCount >= (entry.hopCount + 1))
			{
				if (d_routeTable[idx].hopCount == (entry.hopCount + 1))
				{
					if ((rand()%population.size()) > 0.7 * population.size())
					{
						d_routeTable[idx].nextHop = senderID;
						d_routeTable[idx].hopCount = (entry.hopCount + 1);
					}
				}
				else
				{
					improved = true;
					d_routeTable[idx].nextHop = senderID;
					d_routeTable[idx].hopCount = entry.hopCount + 1;
				}
			}
			++idx;
		}
	}
	return improved;
}

void Agent::sendMessage(Agent &reciever, vector<shared_ptr<Agent> > &population)
{
	++d_messagesTotal;
	d_battery -= 5; //battery cost of sending the message
	int nextHop = d_routeTable[reciever.id()].nextHop;
	if (nextHop != -1)
	{
		if (population[nextHop].get()->routeMessage(reciever, *this, population))
			++d_messagesSent;
	}
	// else
	// {
		// cout << "FAILURE!!!: sendMessage failed due to the reciever being out of range\n";
	// }
}

bool Agent::routeMessage(Agent &reciever, Agent &sender, vector<shared_ptr<Agent> > &population)
{
	if (!(this->wantsToRoute(sender.id())))
	{
		sender.setMemoryAt(this->id(), 0);
		return false;
	}
	else
		sender.setMemoryAt(this->id(), 1);

	if (this->id() != reciever.id())
	{
		++d_messagesRouted;
		d_battery -= 3; //routing cost
		int nextHop = d_routeTable[reciever.id()].nextHop;
		if (nextHop == -1)
			cout << "routeMessage failure due to routing algorithm not working properly\n";
		else
			return population[nextHop].get()->routeMessage(reciever, sender, population);
	}
	else
	{
		++d_messagesRecieved;
		return true;
	}
	return false;
	
}
