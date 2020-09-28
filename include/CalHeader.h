#ifndef CAL_HEADER_H
#define CAL_HEADER_H

#include <string>

#include "WBitmap.h"
#include "FontInfo.h"
#include "Color.h"

class CalHeader
{
public:
    CalHeader(const std::wstring& text, int x, int y, int w, int h, Color color)
    : _text(text), _x(x), _y(y), _w(w), _h(h), _color(color)
    {}

    void renderGraphics(WBitmap& canvas) const;
    void renderText(HWND hwnd, const FontInfo& font) const;

private:
    std::wstring _text;
    int _x;
    int _y;
    int _w;
    int _h;
    Color _color;
};

#endif // CAL_HEADER_H
