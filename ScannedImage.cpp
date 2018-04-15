#include <sane/sane.h>
#include "Scanner.h"
#include "ScannedImage.h"
#include<png++/png.hpp>
#include<iostream>
using namespace std;
ScannedImage::ScannedImage(Scanner* scanner):scanner(scanner), handle(scanner->handle)
{
    // Get parameters for the image
    sane_get_parameters(handle, &this->params);
    cout << this->params.bytes_per_line << " bytes per line" << endl;
    cout << this->params.lines << " lines" << endl;
    switch(params.format)
    {
        case SANE_FRAME_RGB:
        switch(params.depth)
        {
            case 8:
            _GetNextFrame=&ScannedImage::GetNextFrameRGB8;
            break;
            default:break;
        }
        default:
        break;
    }
}
ScannedImage::~ScannedImage()
{

}
bool ScannedImage::GetNextFrame()
{
    return ((*this).*(_GetNextFrame))();
}
#include<iostream>
using namespace std;
bool ScannedImage::GetNextFrameRGB8()
{
    sane_start(handle);
    while(true)
    {
        //cout << x << endl;
        if(x==0)
        {
            //cout << "X is 0, create new array" << endl;
            // Create new array for bytes in this line
            lines.push_back(new unsigned char[this->params.bytes_per_line]);
        }
        int len;
        //cout << "Starting sane_read" << endl;
        //cout << handle << endl;
        //cout << *(unsigned*)(lines[y]+x*sizeof(int)) << endl;
        //cout << (int)sizeof(lines[y]) << endl;
        //cout << &len << endl;
        SANE_Status status = sane_read(handle, lines[y]+x*sizeof(int), this->params.bytes_per_line, &len);
        if(status == SANE_STATUS_EOF)
        {
            return true;
        }
        //cout << len << endl;
        //cout << "Ending sane_read" << endl;
        x++;
        if(x*sizeof(int) == sizeof(*lines[y]))
        {
        //cout << "Out of space..." << endl;
            // Out of space
            x=0;
            y++;
        }
    }
    return true;
}
struct color
{
    char r;
    char g;
    char b;
    char a;
};
extern char exitNow;
void ScannedImage::OutputToFile(const char* output)
{
    cout << "create image (" << params.bytes_per_line/sizeof(color) << ", " << lines.size() << ")" << endl;
    png::image<png::rgb_pixel> image(params.bytes_per_line/sizeof(color), lines.size());
    for(size_t i=0;i<lines.size();i++)
    {
        // Cast to color* so we can loop through it easier
        color* line=(color*)lines[i];
        for(size_t j=0;j<params.bytes_per_line/sizeof(color);j++)
        {
            //cout << "image["<<j<<"]["<<i<<"] = png::rgb_pixel("<<+line->r<<","<<+line->g<<","<<+line->b<<");" << endl;
            image[j][i] = png::rgb_pixel(line->r, line->g, line->b);
        }
    }
    image.write(output);
    exitNow=1;
}