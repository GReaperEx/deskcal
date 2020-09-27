#include "CalIndex.h"

CalIndex::CalIndex(int index)
: _index(index)
{}

void CalIndex::renderGraphics(WBitmap& canvas, int x, int y, int w, int h, Color color) const
{
    WBitmap calSquare(w, h, color);
    calSquare.renderOnBmp(canvas, x, y);
}

void CalIndex::renderText(HWND hwnd, int x, int y, int w, int h, int numSize) const
{
    HFONT myFont = CreateFont(numSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, GREEK_CHARSET, OUT_OUTLINE_PRECIS,
                              CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Times New Roman");
    HDC hdc = GetDC(hwnd);

    SelectObject(hdc, myFont);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    RECT textBox = { x, y, x + w, y + h };
    DrawText(hdc, std::to_string(_index).c_str(), -1, &textBox, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    DeleteObject(myFont);

    ReleaseDC(hwnd, hdc);
}