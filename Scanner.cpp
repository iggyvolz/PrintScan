#include "Scanner.h"
#include <sane/sane.h>
#include <vector>
#include "ScannedImage.h"
using namespace std;
Scanner::~Scanner()
{
	if(scannedImage != nullptr)
	{
		delete scannedImage;
	}
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
const string Scanner::GetIdentifier()
{
    return this->device.name;
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
void* Scanner::_GetCurrentValue(std::size_t index, std::size_t size, int* info, SANE_Status* status)
{
	void* value = malloc(size);
	SANE_Status s = sane_control_option(this->handle, index, SANE_ACTION_GET_VALUE, value, info);
	if (status != nullptr)
	{
		*status = s;
	}
	return value;
}

SANE_Status Scanner::_SetCurrentValue(std::size_t index, void* value, std::size_t size, int* info)
{
	return sane_control_option(this->handle, index, SANE_ACTION_SET_VALUE, value, info);
}
#include <signal.h>
ScannedImage* Scanner::StartScan()
{
	ScannedImage* ptr=new ScannedImage(this);
	this->scannedImage=ptr;
	return ptr;
}