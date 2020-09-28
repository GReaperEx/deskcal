#include "FontInfo.h"
#include "TextUtils.h"

std::ostream& operator<< (std::ostream& outStream, const FontInfo& toWrite)
{
    return write_escaped_string(outStream << "(", toWrite.typeface) << ", " << toWrite.size << ", " << toWrite.weight << ", "
                                          << toWrite.italic << ", " << toWrite.underlined << ", " << toWrite.strikeout << ")";
}

std::istream& operator>> (std::istream& inStream, FontInfo& toRead)
{
    std::string typeface;
    int size, weight;
    bool italic, underlined, strikeout;
    char c[7];

    if (!(read_escaped_string(inStream >> c[0], typeface) >> c[1] >> size >> c[2] >> weight >> c[3] >> italic >> c[4] >> underlined >> c[5] >> strikeout >> c[6])) {
        return inStream;
    }
    if (c[0] != '(' || c[1] != ',' || c[2] != ',' || c[3] != ',' || c[4] != ',' || c[5] != ',' || c[6] != ')' || size <= 0 || weight <= 0 || weight > 1000) {
        inStream.setstate(std::ios::failbit);
        return inStream;
    }

    toRead = { typeface, size, weight, italic, underlined, strikeout };
    return inStream;
}