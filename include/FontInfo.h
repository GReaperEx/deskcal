#ifndef FONT_INFO_H
#define FONT_INFO_H

#include <string>

struct FontInfo
{
    std::string typeface;

    int size;
    int weight;

    bool italic;
    bool underlined;
    bool strikeout;
};

#endif // FONT_INFO_H