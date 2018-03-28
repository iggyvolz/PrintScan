#pragma once
#include <vector>
#include <string>
#include "Screen.h"
#include "MenuScreen.h"
#include "Scanner.h"
#include <curses.h>
class ScannerSelectionScreen : public MenuScreen
{
    public:
        ScannerSelectionScreen(WINDOW*, std::vector<Scanner*>);
        ~ScannerSelectionScreen();
        void OnPress() override;
	private:
		std::vector<Scanner*> scanners;
};