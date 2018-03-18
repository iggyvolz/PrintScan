#pragma once
#include "Screen.h"
#include "MenuScreen.h"
#include <curses.h>
#include "Scanner.h"
class SettingsScreen:public MenuScreen
{
	public:
		SettingsScreen(WINDOW* win, Scanner* scanner);
		~SettingsScreen();
		Scanner* scanner;
		void OnPress() override;
		void UpdateOptions();
	private:
		typedef void(SettingsScreen::*memfunc)();
		// Map of on-screen options to the function that should be run when they are pressed (or nullptr when no function should be called
		std::vector<memfunc> pressFunctions;
		// Map of on-screen options to the scanner option they represent (or nullptr when not a scanner option ex. Start Scan)
		std::vector<const SANE_Option_Descriptor*> scannerOptions;
		// Run on press of Start Scan
		void PressStart();
		// Run on press of Reset Settings
		void PressReset();
		// Creates a screen which selects from a list of options
		void OptionsSelector();
		// Creates a screen which selects from a range
		void RangeSelector();
		// Creates a screen which has a text entry function
		void TextEntry();
		// Presses the button
		void ButtonPress();
};

