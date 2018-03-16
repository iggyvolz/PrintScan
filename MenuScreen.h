#pragma once
#include <string>
#include <vector>
#include "Screen.h"
#include <ncurses.h>
class MenuScreen : Screen
{
    public:
        MenuScreen(WINDOW* win);
        virtual ~MenuScreen();
        void Display();
    protected:
        std::string title;
        std::vector<std::string> funcNames;
        virtual void OnPress();
    private:
        std::size_t currOption=0;
        std::size_t topOption=0;
};