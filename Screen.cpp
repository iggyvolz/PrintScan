#include "Screen.h"
#include <ncurses.h>
Screen::Screen(WINDOW* win)
{
    Screen::win=win;
}
Screen::~Screen()
{
    
}