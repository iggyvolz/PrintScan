#include <curses.h>
#include <sstream>
#include <string>
#include <iostream>
#include "Scanner.h"
#include "ScannerSelectionScreen.h"
#include "Screen.h"
#include "MenuScreen.h"
#include "ScannerEnvironment.h"
#include<map>
using namespace std;
#define DISPLAY_TYPE_MENU 0
char DisplayType=DISPLAY_TYPE_MENU;
WINDOW* win;
Screen* currentScreen;
char exitNow = 0;
int nlines=100;
int ncols=100;
int displayLeft=5;
int displayTop=5;
void gameLoop();
int main(int argc, char *argv[])
{
    ScannerEnvironment* env = new ScannerEnvironment();
    // Initialize ncurses
    initscr();
    // Don't echo keys pressed
    noecho();
    // Hide cursor
    curs_set(0);
    // Echo special characters as one
    win=newwin(nlines,ncols,displayTop,displayLeft);
    //box(win,0,0);
    keypad(win, TRUE);
    wrefresh(win);
    // Set up default screen
    currentScreen=new ScannerSelectionScreen(win,env->scanners);
    gameLoop();
    // Clean up memory
    delete currentScreen;
    delete env;
    endwin();
	return 0;
}
void gameLoop()
{
    while(exitNow==0)
    {
        currentScreen->Display();
    }
}