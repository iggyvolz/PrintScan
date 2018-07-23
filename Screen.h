#pragma once
#include <curses.h>
class Screen
{
    public:
        Screen(WINDOW* win);
		virtual void Display() = 0;
        virtual ~Screen();
    protected:
        WINDOW* win;
};