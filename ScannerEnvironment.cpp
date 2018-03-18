#include "Scanner.h"
#include<vector>
#include <sane/sane.h>
#include<string>
#include "ScannerEnvironment.h"
using namespace std;

// TODO check status codes
ScannerEnvironment::ScannerEnvironment()
{
    int version_code=0;
    sane_init(&version_code, nullptr);
    // Pointer to array of device pointers
    const SANE_Device** devices=nullptr;
    sane_get_devices(&devices, true);
	this->scanners.clear();
    for(;*devices!=0;devices++)
    {
        SANE_Device* device = (SANE_Device*)*devices;
        scanners.push_back(new Scanner(*device));
    }
}
ScannerEnvironment::~ScannerEnvironment()
{
    sane_exit();
}