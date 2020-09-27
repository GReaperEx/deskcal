#include "CalDate.h"

CalDate::CalDate(const Date& _date, const std::string& text, Color color, FontInfo font)
: date(_date), _text(text), _color(color), _font(font)
{}

void CalDate::renderGraphics(WBitmap& canvas, int x, int y, int w, int h) const
{
    WBitmap calSquare(w, h, _color);
    calSquare.renderOnBmp(canvas, x, y);
}

void CalDate::renderText(HWND hwnd, int x, int y, int w, int h, int numSize) const
{
    HFONT myFont = CreateFont(numSize, 0, 0, 0, FW_BOLD, FALSE, FALSE,FALSE, GREEK_CHARSET, OUT_OUTLINE_PRECIS,
                                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, "Times New Roman");
    HDC hdc = GetDC(hwnd);

    SelectObject(hdc, myFont);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    RECT textBox = { x + 5, y + 2, x + w, y + h };
    DrawText(hdc, std::to_string(date.day).c_str(), -1, &textBox, DT_LEFT | DT_TOP | DT_WORDBREAK);
    DeleteObject(myFont);

    myFont = CreateFont(_font.size, 0, 0, 0, _font.weight, _font.italic, _font.underlined, _font.strikeout, GREEK_CHARSET, OUT_OUTLINE_PRECIS,
                        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, _font.typeface.c_str());
    SelectObject(hdc, myFont);
    textBox.top += numSize;
    textBox.left -= 5;
    DrawText(hdc, _text.c_str(), -1, &textBox, DT_LEFT | DT_TOP | DT_WORDBREAK);
    DeleteObject(myFont);

    ReleaseDC(hwnd, hdc);
}

bool CalDate::operator< (const CalDate& other) const
{
    if (date.year < other.date.year) {
        return true;
    }
    if (date.year > other.date.year) {
        return false;
    }

    if (date.month < other.date.month) {
        return true;
    }
    if (date.month > other.date.month) {
        return false;
    }

    return date.day < other.date.day;
}

void CalDate::setColor(Color color)
{
    _color = color;
}

void CalDate::setFont(const FontInfo& font)
{
    _font = font;
}