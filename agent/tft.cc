#include "tft.h"

bool Tft::wantsToRoute(int agent)
{
	return static_cast<bool>(d_memoryTable[agent]);
};
