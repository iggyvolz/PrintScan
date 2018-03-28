#pragma once
#include "MenuScreen.h"
#include <curses.h>
#include "SettingsScreen.h"
#include <sane/sane.h>

class RangeSelectorScreen :
	public Screen
{
public:
	RangeSelectorScreen(WINDOW* win, SettingsScreen* settingsScreen, const SANE_Option_Descriptor* descriptor, size_t currOptionIndex);
	~RangeSelectorScreen();
	void Display() override;
private:
	SettingsScreen * settingsScreen;
	const SANE_Option_Descriptor* descriptor;
	size_t currOptionIndex;
	SANE_String_Const name;
	SANE_Int min;
	SANE_Int max;
	SANE_Int curr;
	SANE_Int step;
};

