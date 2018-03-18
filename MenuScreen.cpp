#include <curses.h>
#include <vector>
#include "MenuScreen.h"
#include<iostream>
using namespace std;
// Constructor with title
MenuScreen::MenuScreen(WINDOW* win, string title) :Screen(win), title(title), options(vector<string>())
{
}
// Constructor without title
MenuScreen::MenuScreen(WINDOW* win) : Screen(win), title(string()), options(vector<string>())
{
}
MenuScreen::~MenuScreen()
{
}
void MenuScreen::Display()
{
    // Clear screen
    wclear(win);
    // Draw title
    mvwaddstr(win, 0, 0, this->title.c_str());
    // Draw on window
    for(std::size_t i=0;i<this->options.size();i++)
    {
        // Print option on window
        if(i==this->currOption)
        {
            // Print cursor
            mvwaddch(win,i+1, 0, '>');
        }
        // Move to right of cursor and print text
        mvwaddstr(win, i+1, 2, this->options.at(i).c_str());

    }
    wrefresh(win);
    int pressed=wgetch(this->win);
    switch(pressed)
    {
        case KEY_UP:
			do
			{
				// Move cursor up
				if (this->currOption == 0)
				{
					// Reset to last option
					this->currOption = this->options.size();
				}
				this->currOption--;
			} while (this->skipOption[this->currOption]); // Keep moving up until we aren't skipping an action
            break;
        case KEY_DOWN:
			do
			{
				// Move cursor down
				this->currOption++;
				if (this->currOption == this->options.size())
				{
					// Reset to first option
					this->currOption = 0;
				}
			} while (this->skipOption[this->currOption]); // Keep moving up until we aren't skipping an action
            break;
        case KEY_ENTER:
        case 10:
            {
                OnPress();
            }
            break;
    }
}

std::size_t MenuScreen::GetCurrentOption()
{
	return this->currOption;
}
