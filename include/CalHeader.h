#ifndef CAL_HEADER_H
#define CAL_HEADER_H

#include <string>

#include "WBitmap.h"
#include "FontInfo.h"
#include "Color.h"

class CalHeader
{
public:
    CalHeader(const std::wstring& text);

    void renderGraphics(WBitmap& canvas, int x, int y, int w, int h, Color color) const;
    void renderText(HWND hwnd, int x, int y, int w, int h, const FontInfo& font) const;

private:
    std::wstring _text;
};

#endif // CAL_HEADER_H
