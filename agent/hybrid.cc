#include "hybrid.h"

bool Hybrid::wantsToRoute(int agent)
{
	if (d_memoryTable[agent] == 1)
		return d_wantsToRoute;
	else
		return false;
};

void Hybrid::setWillingnessToRoute()
{
	if (d_battery < 400 && d_battery < rand()%400)
		d_wantsToRoute = false;
	else
		d_wantsToRoute = true;
};

