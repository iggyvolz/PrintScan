#include "RangeSelectorScreen.h"
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include<iostream>
using namespace std;
RangeSelectorScreen::RangeSelectorScreen(WINDOW* win, SettingsScreen* settingsScreen, const SANE_Option_Descriptor* descriptor, size_t currOptionIndex):Screen(win)
{
	if (descriptor->constraint_type == SANE_Constraint_Type::SANE_CONSTRAINT_RANGE)
	{
		this->min = descriptor->constraint.range->min;
		this->max = descriptor->constraint.range->max;
		this->curr = this->min;
		this->step = descriptor->constraint.range->quant;
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
	cout << __LINE__ << endl;
	// Clear screen
	cout << __LINE__ << endl;
	wclear(win);
	cout << __LINE__ << endl;
	// Draw title
	cout << __LINE__ << endl;
	mvwaddstr(win, 0, 0, this->name);
	cout << __LINE__ << endl;
	// Draw on window
	cout << __LINE__ << endl;
	// Subtract 2 for the borders
	cout << __LINE__ << endl;
	short width = this->win->_maxx - 2;
	cout << __LINE__ << endl;
	mvwaddch(win, 1, 0, '|');
	cout << __LINE__ << endl;
	int numCharsToPrint = this->curr - this->min;
	cout << __LINE__ << endl;
	numCharsToPrint *= width;
	cout << __LINE__ << endl;
	numCharsToPrint /= this->max;
	cout << __LINE__ << endl;
	for (;numCharsToPrint >= 0;numCharsToPrint--)
	{
		cout << __LINE__ << endl;
		waddch(win, '|');
	}
	cout << __LINE__ << endl;
	mvwaddch(win, 1, this->win->_maxx - 1, '|');
	cout << __LINE__ << endl;
	wrefresh(win);
	cout << __LINE__ << endl;
	int pressed = wgetch(this->win);
	cout << __LINE__ << endl;
	switch (pressed)
	{
	case KEY_UP:
		cout << __LINE__ << endl;
		this->curr += this->step;
		cout << __LINE__ << endl;
		if (this->curr > this->max) this->curr = this->max;
		cout << __LINE__ << endl;
		break;
	case KEY_DOWN:
		cout << __LINE__ << endl;
		this->curr -= this->step;
		cout << __LINE__ << endl;
		if (this->curr < this->min) this->curr = this->min;
		cout << __LINE__ << endl;
		break;
	case KEY_ENTER:
	case 10:
		cout << __LINE__ << endl;
		break;
	}
	cout << __LINE__ << endl;
}