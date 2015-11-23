#ifndef GUI_NCURSES_H
#define GUI_NCURSES_H

#include <string.h>
#include <ncurses.h>

// Function prototypes
WINDOW* createWindow(int nlines, int ncols, int y0, int x0, const char* title);

#endif
