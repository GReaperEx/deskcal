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

        Date(int y, int m, int d)
        : year(y), month(m), day(d)
        {}
    } date;

public:
    CalDate(const Date& _date, const std::string& text, Color color, FontInfo font);

    void renderGraphics(WBitmap& canvas, int x, int y, int w, int h) const;
    void renderText(HWND hwnd, int x, int y, int w, int h, int numSize) const;

    bool operator< (const CalDate& other) const;

    void setColor(Color color);
    void setFont(const FontInfo& font);

private:
    std::string _text;
    Color _color;
    FontInfo _font;

};

#endif // CAL_DATE_H
