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
OptionSelectorScreen::OptionSelectorScreen(WINDOW* win, SettingsScreen* settingsScreen, const SANE_Option_Descriptor* descriptor):MenuScreen(win), settingsScreen(settingsScreen), descriptor(descriptor)
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

extern char exitNow;
void OptionSelectorScreen::OnPress()
{
	endwin();
	// Set option number
	switch (descriptor->constraint_type)
	{
		case SANE_CONSTRAINT_WORD_LIST:
			{
				int info;
				SANE_Status status = this->settingsScreen->scanner->SetCurrentValue(this->settingsScreen->GetCurrentOption(), descriptor->constraint.word_list[this->currOption + 1], sizeof(SANE_Word), &info); // First index of word_list is length
				cout << "Info: " << info << ", status: " << status << endl;
			}
			break;
		case SANE_CONSTRAINT_STRING_LIST:
			{
				int info;
				SANE_Status status = this->settingsScreen->scanner->SetCurrentValuePointer(this->settingsScreen->GetCurrentOption(), descriptor->constraint.string_list[this->currOption], strlen(descriptor->constraint.string_list[this->currOption])+1, &info);
				cout << "Info: " << info << ", status: " << status << endl;
			}
			break;
		default:
			// Nonsensical option
			cout << "Error: OptionSelectionScreen with constraint " << descriptor->constraint_type << endl;
			std::exit(1);
			break;
	}
	cout << "Current value: " << this->settingsScreen->scanner->GetCurrentValue<unsigned>(this->settingsScreen->GetCurrentOption()) << endl; // TODO
	exitNow = 1;
	// Revert to settings screen
	//currentScreen = this->settingsScreen;
	//this->settingsScreen->UpdateOptions();
	delete this;
}