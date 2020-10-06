#ifndef CAL_DATE_H
#define CAL_DATE_H

#include <string>

#include "WBitmap.h"
#include "FontInfo.h"
#include "Color.h"
#include "TextUtils.h"

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
    CalDate() {
        _listed = false;
    }
    CalDate(const Date& _date, const std::wstring& text, const Color& color, const FontInfo& font, bool listed = false)
    : date(_date), _text(text), _color(color), _font(font), _listed(listed)
    {}

    void renderGraphics(WBitmap& canvas, int x, int y, int w, int h) const;
    void renderGraphics(HWND hwnd, int x, int y, int w, int h) const;
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

    Color getColor() const {
        return _color;
    }

    void setFont(const FontInfo& font) {
        _font = font;
    }

    const FontInfo& getFont() const {
        return _font;
    }

    HFONT createFont() const {
        return CreateFont(_font.size, 0, 0, 0, _font.weight, _font.italic, _font.underlined, _font.strikeout, GREEK_CHARSET, OUT_OUTLINE_PRECIS,
                          CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, utf8_to_utf16(_font.typeface).c_str());
    }

    const std::wstring& getText() const {
        return _text;
    }

    void setText(const std::wstring& newText) {
        _text = newText;
    }

    bool getListed() const {
        return _listed;
    }

    void setListed(bool newVal) {
        _listed = newVal;
    }

    friend std::ostream& operator<< (std::ostream& outStream, const CalDate& toWrite);
    friend std::istream& operator>> (std::istream& inStream, CalDate& toRead);

private:
    std::wstring _text;
    Color _color;
    FontInfo _font;
    bool _listed;

};

#endif // CAL_DATE_H
