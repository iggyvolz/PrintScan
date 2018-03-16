#pragma once
#include<vector>
#include <sane/sane.h>
#include<string>
class Scanner
{
    public:
        virtual ~Scanner();
        SANE_Status Open();
        const std::string GetName();
        friend class ScannerEnvironment;
    private:
        Scanner(SANE_Device);
        SANE_Handle handle;
        SANE_Device device;
};