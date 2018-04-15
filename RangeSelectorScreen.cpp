#include "RangeSelectorScreen.h"
#include<iostream>
#include<cstring>
extern Screen* currentScreen;
using namespace std;
RangeSelectorScreen::RangeSelectorScreen(WINDOW* win, int min, int max, int step, SANE_String_Const name, size_t currOptionIndex):Screen(win),name(name),min(min),max(max),curr(min),step(step)
{
}
RangeSelectorScreen::RangeSelectorScreen(WINDOW* win, SettingsScreen* settingsScreen, const SANE_Option_Descriptor* descriptor, size_t currOptionIndex):Screen(win), settingsScreen(settingsScreen)
{
	if (descriptor->constraint_type == SANE_Constraint_Type::SANE_CONSTRAINT_RANGE)
	{
		this->min = descriptor->constraint.range->min;
		this->max = descriptor->constraint.range->max;
		this->curr = this->min;
		this->step = descriptor->constraint.range->quant;
		if(this->step == 0) this->step=(this->max-this->min)/20;
		if(this->step==0) this->step=1;
	}
	else
	{
		this->max = 0b0-1;
		this->min = 0b0;
		this->curr = this->min;
		this->step = 1;
	}
	this->name = descriptor->name;
}
RangeSelectorScreen::~RangeSelectorScreen()
{

}
void RangeSelectorScreen::Display()
{
	// Clear screen
	wclear(win);
	// Draw title
	mvwaddstr(win, 0, 0, this->name);
	// Draw on window
	// Subtract 2 for the borders
	short width = this->win->_maxx - 2;
	mvwaddch(win, 1, 0, '|');
	int numCharsToPrint = this->curr - this->min;
	numCharsToPrint *= width;
	numCharsToPrint /= this->max-this->min;
	for (int i=0;i<numCharsToPrint;i++)
	{
		waddch(win, '*');
	}
	mvwaddch(win, 1, this->win->_maxx - 1, '|');
	char* text = new char[6]; // Print at most 5 characters 
	snprintf(text,6,"%d",this->curr);
	int belowPrintingX=numCharsToPrint-(strlen(text)-1)/2;
	if(belowPrintingX<0) belowPrintingX=0;
	if(belowPrintingX>=this->win->_maxx-(int)strlen(text)) belowPrintingX=this->win->_maxx-(int)strlen(text);
	mvwaddstr(win,2,belowPrintingX,text);
	delete text;
	wrefresh(win);
	int pressed = wgetch(this->win);
	switch (pressed)
	{
	case KEY_UP:
		this->curr += this->step;
		if (this->curr > this->max) this->curr = this->max;
		break;
	case KEY_DOWN:
		this->curr -= this->step;
		if (this->curr < this->min) this->curr = this->min;
		break;
	case KEY_ENTER:
	case 10:
		// Set option number
		this->settingsScreen->scanner->SetCurrentValue(currOptionIndex, curr);
		// Revert to settings screen
		currentScreen = this->settingsScreen;
		this->settingsScreen->UpdateOptions();
		delete this;
		break;
	}
}