#include "Scanner.h"
#include <sane/sane.h>
#include <vector>
#include <iostream>
using namespace std;
Scanner::~Scanner()
{

}
Scanner::Scanner(SANE_Device device)
{
    this->device=device;
}
SANE_Status Scanner::Open()
{
    cout << "Trying to open scanner named " << this->device.name << endl;
    
    SANE_Status status = sane_open(this->device.name, &handle);
    return status;
}
const string Scanner::GetName()
{
    return this->device.model;
}