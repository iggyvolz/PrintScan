#pragma once
#include <sane/sane.h>
#include "Scanner.h"
#include <vector>
class Scanner;
// TODO make this work with PNGs, JPEGs, etc
class ScannedImage
{
    public:
        ScannedImage(Scanner* scanner);
        virtual ~ScannedImage();
        // Returns true if the next frame is the last
        void ReadToFile(const char* output);
    private:
        Scanner* scanner;
        SANE_Handle handle;
        size_t x=0;
        size_t y=0;
        std::vector<unsigned char*> lines;
        SANE_Parameters params;
};