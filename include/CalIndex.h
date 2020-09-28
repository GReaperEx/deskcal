#ifndef CAL_INDEX_H
#define CAL_INDEX_H

#include "WBitmap.h"
#include "FontInfo.h"
#include "Color.h"

class CalIndex
{
public:
    CalIndex(int index, int x, int y, int w, int h)
    : _index(index), _x(x), _y(y), _w(w), _h(h)
    {}

    void renderGraphics(WBitmap& canvas, Color color) const;
    void renderText(HWND hwnd, int numSize) const;

private:
    int _index;
    int _x;
    int _y;
    int _w;
    int _h;
};

#endif // CAL_INDEX_H
