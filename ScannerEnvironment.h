#pragma once
#include "Scanner.h"
class ScannerEnvironment
{
    public:
        std::vector<Scanner*> scanners;
        ScannerEnvironment();
        ~ScannerEnvironment();
};