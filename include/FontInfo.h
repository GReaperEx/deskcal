#ifndef FONT_INFO_H
#define FONT_INFO_H

#include <string>
#include <iostream>

struct FontInfo
{
    std::string typeface;

    int size;
    int weight;

    bool italic;
    bool underlined;
    bool strikeout;

    bool operator!= (const FontInfo& other) const {
        return typeface != other.typeface || size != other.size || weight != other.weight ||
               italic != other.italic || underlined != other.underlined || strikeout != other.strikeout;
    }

    friend std::ostream& operator<< (std::ostream& outStream, const FontInfo& toWrite);
    friend std::istream& operator>> (std::istream& inStream, FontInfo& toRead);
};

#endif // FONT_INFO_H