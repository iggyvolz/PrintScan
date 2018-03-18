#include "SettingsScreen.h"
#include "Scanner.h"
#include<string>
#include<iostream>
#include<cstring>
#include <functional>
using namespace std;
// Debug
SettingsScreen::SettingsScreen(WINDOW* win, Scanner* scanner):MenuScreen(win, string("Settings")), scanner(scanner)
{
	// Add "Start Scan" option
	this->options.push_back("Start Scan");
	this->pressFunctions.push_back(&SettingsScreen::PressStart);
	this->scannerOptions.push_back(nullptr);
	this->skipOption.push_back(false);
	// Add "Reset Settings" option
	this->options.push_back("Reset Settings");
	this->pressFunctions.push_back(&SettingsScreen::PressReset);
	this->scannerOptions.push_back(nullptr);
	this->skipOption.push_back(false);
	// Add options from scanner
	for(size_t i=0;i!=this->scanner->Options.size();i++)
	{
		const SANE_Option_Descriptor* option = this->scanner->Options[i];
		if (option->type == SANE_TYPE_GROUP)
		{
			string name = (string)(option->name) + ": ";
			this->options.push_back(name);
			this->pressFunctions.push_back(nullptr);
			this->scannerOptions.push_back(option);
			// Don't allow user to hover this entry
			this->skipOption.push_back(true);
		}
		if (!SANE_OPTION_IS_SETTABLE(option->cap))
		{
			// Can't set by software, no point in printing
			break;
		}
		string name = " " + (string)(option->name);
		this->scannerOptions.push_back(option);
		this->skipOption.push_back(false);
		switch (option->type)
		{
			case SANE_TYPE_BOOL:
				// Push options selector
				this->pressFunctions.push_back(&SettingsScreen::OptionsSelector);
				// Get current value
				{
					SANE_Bool currValue = this->scanner->GetCurrentValue<SANE_Bool>(i);
					if (currValue)
					{
						// Current value is true
						name += " (true)";
					}
					else
					{
						name += " (false)";
					}
				}
				break;
			default:
				// TODO
				break;
		}
		// Add option name
		this->options.push_back(name);
		break;
	}
}


SettingsScreen::~SettingsScreen()
{
}

void SettingsScreen::OnPress()
{
	// Get the approriate function for this keypress
	memfunc x = this->pressFunctions[this->currOption];
	if (x == nullptr) return; // Don't call a function
	// Run the function
#if __cplusplus>201500
	std::invoke(x, this);
#else
	//Pre-2017 standard, create macro
#ifndef CALL_MEMBER_FN
#define CALL_MEMBER_FN(ptrToMember,object,...)  ((*object).*(ptrToMember))(__VA_ARGS__)
#endif
	CALL_MEMBER_FN(x, this);
#endif
}
extern char exitNow;
void SettingsScreen::PressStart()
{
	endwin();
	cout << "Start Scan pressed" << endl;

	// Display all settings
	/*
	for (const SANE_Option_Descriptor* option : this->scanner->Options)
	{
		cout << "Setting " << option->name << " has type " << option->type << " and unit " << option->unit;
		switch (option->constraint_type)
		{
		case SANE_CONSTRAINT_NONE:
			cout << " and no constraints." << endl;
			break;
		case SANE_CONSTRAINT_RANGE:
			cout << " and a range constraint from " << option->constraint.range->min << " to " << option->constraint.range->max << " with quantization " << option->constraint.range->quant << endl;
			break;
		case SANE_CONSTRAINT_WORD_LIST:
		{
			const int * list = option->constraint.word_list;
			int numWords = *list;
			cout << " and " << numWords << " valid words: ";

			for (int i = 0;i < numWords;i++)
			{
				list++;
				cout << *list << ", ";
			}
			cout << endl;
		}
		break;
		case SANE_CONSTRAINT_STRING_LIST:
			cout << " and valid strings: ";
			for (const SANE_String_Const * list = option->constraint.string_list;(*list) != NULL;list++)
			{
				cout << *list << ", ";
			}
			cout << endl;
			break;
		}
	} */
	exitNow = 1;
}

void SettingsScreen::PressReset()
{
	endwin();
	cout << "Reset Settings pressed" << endl;
	exitNow = 1;
}


void SettingsScreen::OptionsSelector()
{

}

void SettingsScreen::RangeSelector()
{

}

void SettingsScreen::TextEntry()
{

}

void SettingsScreen::ButtonPress()
{

}