#ifndef DESKCAL_H
#define DESKCAL_H

#include "WBitmap.h"
#include "CalDate.h"
#include "CalHeader.h"
#include "CalIndex.h"
#include "CalTitle.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

class DeskCalendar
{
public:
    DeskCalendar(HWND hwnd) {
        _hwnd = hwnd;

        time_t nowTime = time(0);
        tm now = *localtime(&nowTime);
        _curDate = CalDate::Date(now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);

        loadConfig();
        loadDates();
    }

    ~DeskCalendar() {
        //saveConfig();
        saveDates();
    }

    bool loadConfig();
    bool saveConfig() const;

    bool loadDates();
    bool saveDates() const;

    void update();
    void render();

    void setCurrentDate(const CalDate::Date& newDate) {
        _curDate = newDate;

        update();
        render();
    }

private:
    struct DatePointer {
        int x;
        int y;
        int w;
        int h;
        CalDate::Date date;
        std::vector<CalDate>* ptr;

        DatePointer(int _x, int _y, int _w, int _h, CalDate::Date _date, std::vector<CalDate>* _ptr)
        : x(_x), y(_y), w(_w), h(_h), date(_date), ptr(_ptr)
        {}
    };

    HWND _hwnd;
    CalDate::Date _curDate;

    std::vector<CalDate> _editedDates; // sorted
    std::vector<CalDate> _dummyDates;  // sorted
    std::vector<DatePointer> _renderedDates;

    CalTitle _renderedTitle;
    std::vector<CalHeader> _renderedHeaders;
    std::vector<CalIndex> _renderedIndices;

    void resetConfig();

    // Config variables start here

    int _headerIndexSize;
    int _titleSize;

    int _marginNarrow;
    int _marginWide;

    int _marginLeft;
    int _marginTop;
    int _marginRight;
    int _marginBottom;

    Color _defaultColor;
    Color _weekendColor;
    Color _monthColor;
    Color _curDayColor;

    int _numberSize;
    FontInfo _defaultFont;

    float _columnSizes[7];
    int _rowAmount;
};


#endif // DESKCAL_H
