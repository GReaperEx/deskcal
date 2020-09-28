#include "CalTitle.h"
#include "TextUtils.h"

void CalTitle::renderGraphics(WBitmap& canvas, Color color) const
{
    WBitmap calSquare(_w, _h, color);
    calSquare.renderOnBmp(canvas, _x, _y);
}

void CalTitle::renderText(HWND hwnd, const FontInfo& font) const
{
    HFONT myFont = CreateFont(font.size, 0, 0, 0, font.weight + 300, font.italic, font.underlined, font.strikeout, GREEK_CHARSET, OUT_OUTLINE_PRECIS,
                              CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, utf8_to_utf16(font.typeface).c_str());
    HDC hdc = GetDC(hwnd);

    SelectObject(hdc, myFont);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    RECT textBox = { _x, _y, _x + _w, _y + _h };
    DrawText(hdc, _text.c_str(), -1, &textBox, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    DeleteObject(myFont);

    ReleaseDC(hwnd, hdc);
}