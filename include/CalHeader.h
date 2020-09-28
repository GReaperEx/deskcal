#ifndef CAL_HEADER_H
#define CAL_HEADER_H

#include <string>

#include "WBitmap.h"
#include "FontInfo.h"
#include "Color.h"

class CalHeader
{
public:
    CalHeader(const std::wstring& text, int x, int y, int w, int h)
    : _text(text), _x(x), _y(y), _w(w), _h(h)
    {}

    void renderGraphics(WBitmap& canvas, Color color) const;
    void renderText(HWND hwnd, const FontInfo& font) const;

private:
    std::wstring _text;
    int _x;
    int _y;
    int _w;
    int _h;
};

#endif // CAL_HEADER_H
