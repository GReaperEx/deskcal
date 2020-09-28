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

class DeskCalendar
{
public:
    DeskCalendar(int week) {
        _curWeek = week;
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

    void update(HWND hwnd);
    void render(HWND hwnd);

    void setCurrentWeek(int week) {
        _curWeek = week;
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

    int _curWeek;
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
