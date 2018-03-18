#include "OptionSelectorScreen.h"
#include "SettingsScreen.h"
#include "Scanner.h"
#include<string>
#include<iostream>
#include<cstring>
#include <functional>
#include <curses.h>
using namespace std;
extern Screen* currentScreen;
SettingsScreen::SettingsScreen(WINDOW* win, Scanner* scanner):MenuScreen(win, string("Settings")), scanner(scanner)
{
	SettingsScreen::UpdateOptions();
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
	currentScreen = new OptionSelectorScreen(this->win, this, this->scannerOptions[this->currOption]);
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

void SettingsScreen::UpdateOptions()
{
	this->options.clear();
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
	// Create temporary array to map constraint types and data types to selectors
	// Indexed by (type<<2)|constraint - unique because constraint is two bits and type is three
	memfunc selectors[20] = {}; // Initialized to 0==nullptr
								// Define a macro to get/set a selector
#define selector(type,constraint) selectors[(type<<2)|constraint]

								// Set the selectors
	selector(SANE_TYPE_BOOL, SANE_CONSTRAINT_NONE) = &SettingsScreen::ButtonPress;
	selector(SANE_TYPE_INT, SANE_CONSTRAINT_NONE) = &SettingsScreen::RangeSelector;
	selector(SANE_TYPE_INT, SANE_CONSTRAINT_RANGE) = &SettingsScreen::RangeSelector;
	selector(SANE_TYPE_INT, SANE_CONSTRAINT_WORD_LIST) = &SettingsScreen::OptionsSelector;
	selector(SANE_TYPE_FIXED, SANE_CONSTRAINT_NONE) = &SettingsScreen::RangeSelector;
	selector(SANE_TYPE_FIXED, SANE_CONSTRAINT_RANGE) = &SettingsScreen::RangeSelector;
	selector(SANE_TYPE_FIXED, SANE_CONSTRAINT_WORD_LIST) = &SettingsScreen::OptionsSelector;
	selector(SANE_TYPE_STRING, SANE_CONSTRAINT_NONE) = &SettingsScreen::TextEntry;
	selector(SANE_TYPE_STRING, SANE_CONSTRAINT_STRING_LIST) = &SettingsScreen::OptionsSelector;
	selector(SANE_TYPE_BUTTON, SANE_CONSTRAINT_NONE) = &SettingsScreen::ButtonPress;

	// Add options from scanner
	for (size_t i = 0;i != this->scanner->Options.size();i++)
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
			continue;
		}
		if (!SANE_OPTION_IS_SETTABLE(option->cap))
		{
			// Can't set by software, no point in printing
			continue;
		}
		string name = " " + (string)(option->name) + " ";
		// Set name
		this->scannerOptions.push_back(option);
		// Set as not skipped
		this->skipOption.push_back(false);
		// Set selector
		this->pressFunctions.push_back(selector(option->type, option->constraint_type));

		// Display current value to user
		endwin();
		switch (option->type)
		{
		case SANE_TYPE_BOOL:
			if (scanner->GetCurrentValue<SANE_Bool>(i))
			{
				name += " (true)";
			}
			else
			{
				name += " (false)";
			}
			break;
		case SANE_TYPE_INT:
			name += " (" + std::to_string(scanner->GetCurrentValue<SANE_Int>(i)) + ")";
			break;
		case SANE_TYPE_FIXED:
			name += " (" + std::to_string(SANE_UNFIX(scanner->GetCurrentValue<SANE_Int>(i))) + ")";
			break;
		case SANE_TYPE_STRING:
		{
			char* _value = scanner->GetCurrentValuePointer<char>(i, option->size);
			string value = string(_value);
			delete _value;
			cout << "Got the value" << endl;
			//cout << (void*)value << endl;
			//cout <<  *value << endl;
			name += " (" + string(value) + ")";
		}
		break;
		default:
			// Don't display anything for button or group
			break;
		}

		// Add option name (with value)
		this->options.push_back(name);
	}
}