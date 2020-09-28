#ifndef CAL_DATE_H
#define CAL_DATE_H

#include <string>

#include "WBitmap.h"
#include "FontInfo.h"
#include "Color.h"

class CalDate
{
public:
    struct Date
    {
        int year;
        int month;
        int day;

        Date() {}
        Date(int y, int m, int d)
        : year(y), month(m), day(d)
        {}

        bool operator== (const Date& other) const {
            return year == other.year && month == other.month && day == other.day;
        }

        bool operator< (const Date& other) const;

        friend std::ostream& operator<< (std::ostream& outStream, const Date& toWrite);
        friend std::istream& operator>> (std::istream& inStream, Date& toRead);
    } date;

public:
    CalDate() {}
    CalDate(const Date& _date, const std::wstring& text, const Color& color, const FontInfo& font)
    : date(_date), _text(text), _color(color), _font(font)
    {}

    void renderGraphics(WBitmap& canvas, int x, int y, int w, int h) const;
    void renderText(HWND hwnd, int x, int y, int w, int h, int numSize) const;

    bool operator< (const Date& otherDate) const {
        return date < otherDate;
    }
    bool operator< (const CalDate& other) const {
        return date < other.date;
    }

    void setColor(Color color) {
        _color = color;
    }

    void setFont(const FontInfo& font) {
        _font = font;
    }

    friend std::ostream& operator<< (std::ostream& outStream, const CalDate& toWrite);
    friend std::istream& operator>> (std::istream& inStream, CalDate& toRead);

private:
    std::wstring _text;
    Color _color;
    FontInfo _font;

};

#endif // CAL_DATE_H
