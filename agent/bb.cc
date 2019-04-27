#include "bb.h"

void Bb::setWillingnessToRoute()
{
	if (d_battery < 500 && d_battery < rand()%500)
		d_wantsToRoute = false;
	else
		d_wantsToRoute = true;
};

