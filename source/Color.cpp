#include "Color.h"

std::ostream& operator<< (std::ostream& outStream, const Color& toWrite)
{
    return outStream << "(" << (int)toWrite.r << ", " << (int)toWrite.g << ", " << (int)toWrite.b << ", " << (int)toWrite.a << ")";
}

std::istream& operator>> (std::istream& inStream, Color& toRead)
{
    int r, g, b, a;
    int c1, c2, c3, c4, c5;

    if (!(inStream >> c1 >> r >> c2 >> g >> c3 >> b >> c4 >> a >> c5)) {
        return inStream;
    }
    if (c1 != '(' || c5 != ')' || c2 != ',' || c3 != ',' || c4 != ',' ||
        r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255 || a < 0 || a > 255) {
        inStream.setstate(std::ios::failbit);
        return inStream;
    }

    toRead.r = r;
    toRead.g = g;
    toRead.b = b;
    toRead.a = a;

    return inStream;
}