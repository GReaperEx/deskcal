#include "CalHeader.h"

CalHeader::CalHeader(const std::string& text)
: _text(text)
{}

void CalHeader::renderGraphics(WBitmap& canvas, int x, int y, int w, int h, Color color) const {
    WBitmap calSquare(w, h, color);
    calSquare.renderOnBmp(canvas, x, y);
}

void CalHeader::renderText(HWND hwnd, int x, int y, int w, int h, const FontInfo& font) const {
    HFONT myFont = CreateFont(font.size, 0, 0, 0, font.weight + 300, font.italic, font.underlined, font.strikeout, GREEK_CHARSET, OUT_OUTLINE_PRECIS,
                              CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, font.typeface.c_str());
    HDC hdc = GetDC(hwnd);

    SelectObject(hdc, myFont);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    RECT textBox = { x, y, x + w, y + h };
    DrawText(hdc, _text.c_str(), -1, &textBox, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    DeleteObject(myFont);

    ReleaseDC(hwnd, hdc);
}