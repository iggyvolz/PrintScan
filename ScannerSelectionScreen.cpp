#include "ScannerSelectionScreen.h"
#include "Scanner.h"
#include "SettingsScreen.h"
#include "MenuScreen.h"
#include <iostream>
using namespace std;
extern Screen* currentScreen;
ScannerSelectionScreen::ScannerSelectionScreen(WINDOW* win, std::vector<Scanner*>scanners):MenuScreen(win, string("Select Scanner")), scanners(scanners)
{
    for(Scanner* scanner : scanners)
    {
		// Add scanner name
		this->options.push_back(scanner->GetName());
		// Don't skip any scanners
		this->skipOption.push_back(false);
    }
}
ScannerSelectionScreen::~ScannerSelectionScreen()
{
}
void ScannerSelectionScreen::OnPress()
{
	// Open scanner
	this->scanners[this->currOption]->Open();
	currentScreen = new SettingsScreen(this->win, this->scanners[this->currOption]);
	delete this;
}