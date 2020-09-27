#ifndef CAL_INDEX_H
#define CAL_INDEX_H

#include "WBitmap.h"
#include "FontInfo.h"
#include "Color.h"

class CalIndex
{
public:
    CalIndex(int index);

    void renderGraphics(WBitmap& canvas, int x, int y, int w, int h, Color color) const;
    void renderText(HWND hwnd, int x, int y, int w, int h, int numSize) const;

private:
    int _index;
};

#endif // CAL_INDEX_H
