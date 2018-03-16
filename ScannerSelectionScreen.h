#pragma once
#include <vector>
#include <string>
#include <vector>
#include "Screen.h"
#include "MenuScreen.h"
#include "Scanner.h"
#include <ncurses.h>
class ScannerSelectionScreen : MenuScreen
{
    public:
        ScannerSelectionScreen(WINDOW*, std::vector<Scanner>);
        ~ScannerSelectionScreen();
        void OnPress();
};