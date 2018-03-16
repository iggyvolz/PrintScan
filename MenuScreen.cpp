#include <curses.h>
#include <vector>
#include "MenuScreen.h"
using namespace std;
MenuScreen::MenuScreen(WINDOW* win):Screen(win)
{
    this->funcNames=vector<string>();
    this->title=string("TITLE HERE");
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
    for(std::size_t i=0;i<this->funcNames.size();i++)
    {
        // Print option on window
        if(i==this->currOption)
        {
            // Print cursor
            mvwaddch(win,i+1, 0, '>');
        }
        // Move to right of cursor and print text
        mvwaddstr(win, i+1, 2, this->funcNames.at(i).c_str());

    }
    wrefresh(win);
    int pressed=wgetch(this->win);
    switch(pressed)
    {
        case KEY_UP:
            // Move cursor up
            if(this->currOption == 0)
            {
                // Reset to last option
                this->currOption=this->funcNames.size();
            }
            this->currOption--;
            break;
        case KEY_DOWN:
            // Move cursor down
            this->currOption++;
            if(this->currOption == this->funcNames.size())
            {
                this->currOption=0;
            }
            break;
        case KEY_ENTER:
        case 10:
            {
                OnPress();
            }
            break;
    }
}
void MenuScreen::OnPress()
{
    
}