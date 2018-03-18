#pragma once
#include <string>
#include <vector>
#include "Screen.h"
#include <curses.h>
class MenuScreen : public Screen
{
    public:
		MenuScreen(WINDOW* win, std::string title);
		MenuScreen(WINDOW* win);
        virtual ~MenuScreen();
        void Display() override;
		std::size_t GetCurrentOption();
    protected:
		// Title to display on the menu screen
        std::string title;
		// Name of each option
        std::vector<std::string> options;
		// Whether or not to skip the option when scrolling (e.g. for a title)
		std::vector<bool> skipOption;
		// Function called when an option is pressed
		virtual void OnPress() = 0;
		// Which option is currently hovered
		std::size_t currOption = 0;
		// The option on the top of the screen
		std::size_t topOption = 0;
};