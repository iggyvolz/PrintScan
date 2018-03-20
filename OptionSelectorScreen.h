#pragma once
#include "MenuScreen.h"
#include <curses.h>
#include "SettingsScreen.h"
#include <sane/sane.h>

class OptionSelectorScreen :
	public MenuScreen
{
	public:
		OptionSelectorScreen(WINDOW* win, SettingsScreen* settingsScreen, const SANE_Option_Descriptor* descriptor, size_t currOptionIndex);
		~OptionSelectorScreen();
		void OnPress() override;
	private:
		SettingsScreen* settingsScreen;
		const SANE_Option_Descriptor* descriptor;
		size_t currOptionIndex;
};

