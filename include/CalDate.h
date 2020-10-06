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
        _color = nullptr;
        _font = nullptr;
    }
    CalDate(const Date& _date, const std::wstring& text)
    : date(_date), _text(text) {
        _listed = false;
        _color = nullptr;
        _font = nullptr;
    }
    CalDate(const CalDate& other) {
        date = other.date;
        _text = other._text;
        if (other._color) {
            _color = new Color(*other._color);
        } else {
            _color = nullptr;
        }
        if (other._font) {
            _font = new FontInfo(*other._font);
        } else {
            _font = nullptr;
        }
        _listed = other._listed;
    }

    CalDate& operator= (const CalDate& other) {
        date = other.date;
        _text = other._text;
        if (other._color) {
            _color = new Color(*other._color);
        } else {
            _color = nullptr;
        }
        if (other._font) {
            _font = new FontInfo(*other._font);
        } else {
            _font = nullptr;
        }
        _listed = other._listed;

        return *this;
    }

    ~CalDate() {
        if (_color) {
            delete _color;
        }
        if (_font) {
            delete _font;
        }
    }

    void renderGraphics(WBitmap& canvas, int x, int y, int w, int h, const Color& defaultColor, const Color& weekendColor) const;
    void renderGraphics(HWND hwnd, int x, int y, int w, int h, const Color& defaultColor, const Color& weekendColor) const;
    void renderText(HWND hwnd, int x, int y, int w, int h, int numSize, const FontInfo& defaultFont) const;

    bool operator< (const Date& otherDate) const {
        return date < otherDate;
    }
    bool operator< (const CalDate& other) const {
        return date < other.date;
    }

    void setColor(Color color) {
        if (!_color) {
            _color = new Color(color);
        } else {
            *_color = color;
        }
    }

    const Color* getColor() const {
        return _color;
    }

    void setFont(const FontInfo& font) {
        if (!_font) {
            _font = new FontInfo(font);
        } else {
            *_font = font;
        }
        *_font = font;
    }

    const FontInfo* getFont() const {
        return _font;
    }

    HFONT createFont(const FontInfo& defaultFont) const {
        if (_font) {
            return CreateFont(_font->size, 0, 0, 0, _font->weight, _font->italic, _font->underlined, _font->strikeout, GREEK_CHARSET, OUT_OUTLINE_PRECIS,
                              CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, utf8_to_utf16(_font->typeface).c_str());
        }
        return CreateFont(defaultFont.size, 0, 0, 0, defaultFont.weight, defaultFont.italic, defaultFont.underlined, defaultFont.strikeout,
                          GREEK_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, utf8_to_utf16(defaultFont.typeface).c_str());
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
    Color* _color;
    FontInfo* _font;
    bool _listed;

};

#endif // CAL_DATE_H
