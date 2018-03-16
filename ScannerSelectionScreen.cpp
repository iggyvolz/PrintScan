#include "ScannerSelectionScreen.h"
#include "Scanner.h"
#include "MenuScreen.h"
ScannerSelectionScreen::ScannerSelectionScreen(WINDOW* win, std::vector<Scanner>scanners):MenuScreen(win)
{
    for(Scanner& scan : scanners)
    {
        this->funcNames.push_back(scan.GetName());
    }
}
ScannerSelectionScreen::~ScannerSelectionScreen()
{

}
void ScannerSelectionScreen::OnPress()
{
    
}