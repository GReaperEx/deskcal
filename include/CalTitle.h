#ifndef CAL_TITLE_H
#define CAL_TITLE_H

#include <string>

#include "WBitmap.h"
#include "Color.h"
#include "FontInfo.h"

class CalTitle
{
public:
    CalTitle() {}
    CalTitle(const std::wstring& text, int x, int y, int w, int h)
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

#endif // CAL_TITLE_H