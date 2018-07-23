#include <sane/sane.h>
#include "Scanner.h"
#include "ScannedImage.h"
#include<png++/png.hpp>
#include<iostream>
#include<fstream>
#include <stdlib.h>
using namespace std;
extern char exitNow;
ScannedImage::ScannedImage(Scanner* scanner):scanner(scanner), handle(scanner->handle)
{
    // Get parameters for the image
    sane_get_parameters(handle, &this->params);
    if(this->params.format != SANE_FRAME_RGB && this->params.depth != 8)
    {
        std::cout << "Scanner not supported" << std::endl;
        exitNow=1;
        return;
    }
    //std::cout << "Format: " << this->params.format << std::endl;
    //std::cout << "Last: " << this->params.last_frame << std::endl;
    //std::cout << "Bytes per line: " << this->params.bytes_per_line << std::endl;
    //std::cout << "Pixels per line: " << this->params.pixels_per_line << std::endl;
    //std::cout << "Lines: " << this->params.lines << std::endl;
    //std::cout << "Depth: " << this->params.depth << std::endl;
}
ScannedImage::~ScannedImage()
{

}
struct Color
{
    char r;
    char g;
    char b;
};
void ScannedImage::ReadToFile(const char* file)
{
    sane_start(handle);
    SANE_Byte* buff=new SANE_Byte[this->params.bytes_per_line*this->params.lines];
    SANE_Byte* currBuff=buff;
    while(true)
    {
        SANE_Int len;
        SANE_Status status = sane_read(handle, currBuff, 2048, &len);
        // Move currBuff pointer forwards
        currBuff+=len;
        if(status == SANE_STATUS_EOF)
        {
            break;
        }
    }
    png::image<png::rgb_pixel> image(this->params.pixels_per_line, this->params.lines);
    char* cBuff=(char*)buff;
    for(SANE_Int currLine=0;currLine<this->params.lines;currLine++)
    {
        for(SANE_Int currPixel=0;currPixel<this->params.pixels_per_line;currPixel++)
        {
            char r=*(cBuff++);
            char g=*(cBuff++);
            char b=*(cBuff++);
            image[currLine][currPixel]=png::rgb_pixel(r,g,b);
        }
    }
    image.write(file);
}