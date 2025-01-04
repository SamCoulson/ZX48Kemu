#ifndef INC_DEBUGGER_H
#define INC_DEBUGGER_H

#include <stdint.h>
#include <stdbool.h>

extern bool paused;

extern char breakpoint[5];

extern bool stepping;

void checkBreakPointHit(uint16_t pc);

#endif
