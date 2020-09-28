#include "CalIndex.h"

void CalIndex::renderGraphics(WBitmap& canvas, Color color) const
{
    WBitmap calSquare(_w, _h, color);
    calSquare.renderOnBmp(canvas, _x, _y);
}

void CalIndex::renderText(HWND hwnd, int numSize) const
{
    HFONT myFont = CreateFont(numSize, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, GREEK_CHARSET, OUT_OUTLINE_PRECIS,
                              CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Times New Roman");
    HDC hdc = GetDC(hwnd);

    SelectObject(hdc, myFont);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    RECT textBox = { _x, _y, _x + _w, _y + _h };
    DrawText(hdc, std::to_wstring(_index).c_str(), -1, &textBox, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    DeleteObject(myFont);

    ReleaseDC(hwnd, hdc);
}