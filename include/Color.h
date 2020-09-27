#ifndef COLOR_H
#define COLOR_H

#include "WBitmap.h"

struct Color
{
    BYTE r;
    BYTE g;
    BYTE b;
    BYTE a;

    Color()
    : r(0), g(0), b(0), a(0)
    {}

    Color(int _r, int _g, int _b, int _a)
    : r(_r), g(_g), b(_b), a(_a)
    {}

    operator WBitmap::BGRA() const {
        return WBitmap::BGRA(r, g, b, a);
    }
};

#endif // COLOR_H