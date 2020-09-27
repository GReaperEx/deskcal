#ifndef CAL_TITLE_H
#define CAL_TITLE_H

#include <string>

#include "WBitmap.h"
#include "Color.h"
#include "FontInfo.h"

class CalTitle
{
public:
    CalTitle(const std::wstring& text)
    : _text(text)
    {}

    void renderGraphics(WBitmap& canvas, int x, int y, int w, int h, Color color) const;
    void renderText(HWND hwnd, int x, int y, int w, int h, const FontInfo& font) const;

private:
    std::wstring _text;
};

#endif // CAL_TITLE_H