#include "CalDate.h"

#include "TextUtils.h"

#include <sstream>

void CalDate::renderGraphics(WBitmap& canvas, int x, int y, int w, int h) const
{
    WBitmap calSquare(w, h, _color);
    calSquare.renderOnBmp(canvas, x, y);
}

void CalDate::renderGraphics(HWND hwnd, int x, int y, int w, int h) const
{
    WBitmap calSquare(w, h, _color);
    calSquare.renderOnWnd(hwnd, x, y);
}

void CalDate::renderText(HWND hwnd, int x, int y, int w, int h, int numSize) const
{
    HFONT myFont = CreateFont(numSize, 0, 0, 0, FW_BOLD, FALSE, FALSE,FALSE, GREEK_CHARSET, OUT_OUTLINE_PRECIS,
                                CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Times New Roman");
    HDC hdc = GetDC(hwnd);

    SelectObject(hdc, myFont);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    RECT textBox = { x + 5, y + 2, x + w, y + h };
    DrawText(hdc, std::to_wstring(date.day).c_str(), -1, &textBox, DT_LEFT | DT_TOP);
    DeleteObject(myFont);

    myFont = CreateFont(_font.size, 0, 0, 0, _font.weight, _font.italic, _font.underlined, _font.strikeout, GREEK_CHARSET, OUT_OUTLINE_PRECIS,
                        CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, utf8_to_utf16(_font.typeface).c_str());
    SelectObject(hdc, myFont);
    textBox.top += numSize;
    textBox.left -= 5;

    if (_listed) {
        int totalHeight = 0;
        int index = 1;
        std::wstringstream stream(_text);
        std::wstring line;
        while (std::getline(stream, line)) {
            DrawText(hdc, (std::to_wstring(index) + L")").c_str(), -1, &textBox, DT_LEFT | DT_TOP);
            textBox.left += numSize;
            if (textBox.left > textBox.right) {
                textBox.left = textBox.right;
            }
            int textHeight = DrawText(hdc, line.c_str(), -1, &textBox, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_NOPREFIX) - _font.size;
            totalHeight += textHeight;

            if (totalHeight > h - numSize) {
                DrawText(hdc, L"--------------------------------", -1, &textBox, DT_RIGHT);
                break;
            }
            textBox.top += textHeight;
            ++index;
            textBox.left -= numSize;
        }
    } else {
        int textHeight = DrawText(hdc, _text.c_str(), -1, &textBox, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_WORD_ELLIPSIS | DT_NOPREFIX);
        if (textHeight > h - numSize) {
            textBox = { x, y + h - numSize, x + w, y + h };
            DrawText(hdc, L"--------------------------------", -1, &textBox, DT_RIGHT);
        }
    }

    DeleteObject(myFont);
    ReleaseDC(hwnd, hdc);
}

bool CalDate::Date::operator< (const Date& other) const
{
    if (year < other.year) {
        return true;
    }
    if (year > other.year) {
        return false;
    }

    if (month < other.month) {
        return true;
    }
    if (month > other.month) {
        return false;
    }

    return day < other.day;
}

std::ostream& operator<< (std::ostream& outStream, const CalDate& toWrite)
{
    return write_escaped_string(outStream << '(' << toWrite.date << ", " << toWrite._font << ", " << toWrite._color << ", ", utf16_to_utf8(toWrite._text)) << ')';
}

std::istream& operator>> (std::istream& inStream, CalDate& toRead)
{
    CalDate::Date date;
    FontInfo font;
    Color color;
    std::string text;
    char c[5];

    if (!read_escaped_string(inStream >> c[0] >> date >> c[1] >> font >> c[2] >> color >> c[3], text)) {
        return inStream;
    }

    toRead.date = date;
    toRead._font = font;
    toRead._color = color;
    toRead._text = utf8_to_utf16(text);

    return inStream;
}

std::ostream& operator<< (std::ostream& outStream, const CalDate::Date& toWrite)
{
    return outStream << '(' << toWrite.year << ", " << toWrite.month << ", " << toWrite.day << ')';
}

std::istream& operator>> (std::istream& inStream, CalDate::Date& toRead)
{
    int y, m, d;
    char c[4];

    if (!(inStream >> c[0] >> y >> c[1] >> m >> c[2] >> d >> c[3])) {
        return inStream;
    }
    if (c[0] != '(' || c[1] != ',' || c[2] != ',' || c[3] != ')' ||
        y < 1900 || m <= 0 || m > 12 || d <= 0 || d > 31) {
        inStream.setstate(std::ios::failbit);
        return inStream;
    }

    toRead = { y, m, d };
    return inStream;
}
