#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/debugger.h"

bool paused = false;
char breakpoint[5] = "0000";

void checkBreakPointHit( uint16_t pc )
{
	char programCounterAddr[5];
	sprintf(programCounterAddr, "%04X", pc);

	if (strcmp(breakpoint, programCounterAddr) == 0)
	{
		if(strcmp(breakpoint, "0000") == 0)
		{
			// avoid pausing straight away
			// printf("ignoring PC");
		}
		else
		{
			//printf("**********Breakpoint %s hit", breakpoint);
			paused = true;
		}
	}
}
