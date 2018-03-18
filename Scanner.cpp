#include "Scanner.h"
#include <sane/sane.h>
#include <vector>
using namespace std;
Scanner::~Scanner()
{

}
Scanner::Scanner(SANE_Device device):device(device)
{
	//this->GetOptions();
}
SANE_Status Scanner::Open()
{
    SANE_Status status = sane_open(this->device.name, &handle);
	this->GetOptions();
	return status;
}
const string Scanner::GetName()
{
    return this->device.model;
}
void Scanner::GetOptions()
{

	// Get scanner options
	this->Options.clear();
	for (SANE_Int i = 0;;i++)
	{
		const SANE_Option_Descriptor* opt = sane_get_option_descriptor(this->handle, i);
		if (opt != NULL)
		{
			this->Options.push_back(opt);
		}
		else
		{
			// No more options
			break;
		}
	}
}
void* Scanner::_GetCurrentValue(std::size_t index, std::size_t size, int* info)
{
	void* value = malloc(size);
	sane_control_option(this->handle, index, SANE_ACTION_GET_VALUE, value, info);
	return value;
}
