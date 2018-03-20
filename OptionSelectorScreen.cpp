#include "OptionSelectorScreen.h"
#include <curses.h>
#include "MenuScreen.h"
#include "SettingsScreen.h"
#include <sane/sane.h>
#include<iostream>
#include<string>
#include<cstring>
using namespace std;
extern Screen* currentScreen;
OptionSelectorScreen::OptionSelectorScreen(WINDOW* win, SettingsScreen* settingsScreen, const SANE_Option_Descriptor* descriptor, size_t currOptionIndex):MenuScreen(win), settingsScreen(settingsScreen), descriptor(descriptor), currOptionIndex(currOptionIndex)
{
	string suffixes[] = { "","px"," bits","mm"," dpi","%"," microseconds" };
	string suffix = suffixes[descriptor->unit];
	// Set up options
	switch (descriptor->constraint_type)
	{
		case SANE_CONSTRAINT_WORD_LIST:
			{
				// Display options
				const SANE_Word* word = descriptor->constraint.word_list;
				int length = *word;
				for (int i = 0;i < length;i++)
				{
					SANE_Word cword = *(++word);
					if (descriptor->type == SANE_TYPE_FIXED)
					{
						// Add unfixed value
						this->options.push_back(std::to_string(SANE_UNFIX(cword))+suffix);
						this->skipOption.push_back(false);
					}
					else
					{
						// Add the number
						this->options.push_back(std::to_string(cword)+suffix);
						this->skipOption.push_back(false);
					}
				}
			}
			break;
		case SANE_CONSTRAINT_STRING_LIST:
			{

				// Display names
				const SANE_String_Const* string = descriptor->constraint.string_list;
				while (*string != NULL)
				{
					this->options.push_back(*string+suffix);
					this->skipOption.push_back(false);
					string++;
				}
			}
			break;
		default:
			// Nonsensical option
			cout << "Error: OptionSelectionScreen with constraint " << descriptor->constraint_type << endl;
			std::exit(1);
			break;
	}
}


OptionSelectorScreen::~OptionSelectorScreen()
{
}
#include<csignal>
extern char exitNow;
void OptionSelectorScreen::OnPress()
{
	char* ptr = this->settingsScreen->scanner->GetCurrentValuePointer<char>(currOptionIndex, descriptor->size);
	string val = string(ptr);
	delete ptr;
	// Set option number
	switch (descriptor->constraint_type)
	{
		case SANE_CONSTRAINT_WORD_LIST:
			{
				SANE_Word thing = descriptor->constraint.word_list[this->currOption + 1];
				this->settingsScreen->scanner->SetCurrentValue(currOptionIndex, thing); // First index of word_list is length
			}
			break;
		case SANE_CONSTRAINT_STRING_LIST:
			{
				SANE_String_Const thing = descriptor->constraint.string_list[this->currOption];
				this->settingsScreen->scanner->SetCurrentValuePointer(currOptionIndex, thing);
			}
			break;
		default:
			// Nonsensical option
			cout << "Error: OptionSelectionScreen with constraint " << descriptor->constraint_type << endl;
			std::exit(1);
			break;
	}
	// Revert to settings screen
	currentScreen = this->settingsScreen;
	this->settingsScreen->UpdateOptions();
	delete this;
}