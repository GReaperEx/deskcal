#ifndef DESKCAL_H
#define DESKCAL_H

#include "WBitmap.h"
#include "CalDate.h"
#include "CalHeader.h"
#include "CalIndex.h"
#include "CalTitle.h"
#include "CalButton.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

class DeskCalendar
{
public:
    DeskCalendar(HWND hwnd) {
        _selected = nullptr;
        _editWnd = NULL;
        _hwnd = hwnd;

        time_t nowTime = time(0);
        tm now = *localtime(&nowTime);
        _curDate = CalDate::Date(now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);

        loadConfig();
        loadDates();

        closeButton.load(L"closeButton.png");
        nextButton.load(L"nextButton.png");
        prevButton.load(L"prevButton.png");
        todayButton.load(L"todayButton.png");
        settingsButton.load(L"settingsButton.png");
    }

    bool loadConfig();
    bool saveConfig() const;

    bool loadDates();
    bool saveDates() const;

    void update();
    void render();

    void handleInput(HWND hwnd, UINT msg, WPARAM wParal, LPARAM lParam) {
        switch (msg)
        {
        case WM_LBUTTONDOWN:
            closeButton.onLMBdown(LOWORD(lParam), HIWORD(lParam));
            nextButton.onLMBdown(LOWORD(lParam), HIWORD(lParam));
            todayButton.onLMBdown(LOWORD(lParam), HIWORD(lParam));
            prevButton.onLMBdown(LOWORD(lParam), HIWORD(lParam));
            settingsButton.onLMBdown(LOWORD(lParam), HIWORD(lParam));

            onClick(LOWORD(lParam), HIWORD(lParam));
        break;
        case WM_LBUTTONUP:
            if (closeButton.onLMBup(LOWORD(lParam), HIWORD(lParam))) {
                DestroyWindow(hwnd);
            } else if (nextButton.onLMBup(LOWORD(lParam), HIWORD(lParam))) {
                onClickNext();
            } else if (todayButton.onLMBup(LOWORD(lParam), HIWORD(lParam))) {
                onClickToday();
            } else if (prevButton.onLMBup(LOWORD(lParam), HIWORD(lParam))) {
                onClickPrev();
            } else if (settingsButton.onLMBup(LOWORD(lParam), HIWORD(lParam))) {
                onClickSettings();
            }
        break;
        case WM_LBUTTONDBLCLK:
            onClick(LOWORD(lParam), HIWORD(lParam));
        break;
        }
    }

    void setCurrentDate(const CalDate::Date& newDate) {
        _curDate = newDate;

        update();
        render();
    }

private:
    void onClickNext();
    void onClickToday();
    void onClickPrev();
    void onClickSettings();
    void onClick(int x, int y);
    void onChar(wchar_t wc);

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

    DatePointer* _selected;
    HWND _editWnd;

    HWND _hwnd;
    CalDate::Date _curDate;

    std::vector<CalDate> _editedDates; // sorted
    std::vector<CalDate> _dummyDates;  // sorted
    std::vector<DatePointer> _renderedDates;

    CalTitle _renderedTitle;
    std::vector<CalHeader> _renderedHeaders;
    std::vector<CalIndex> _renderedIndices;

    CalButton closeButton;
    CalButton nextButton;
    CalButton prevButton;
    CalButton todayButton;
    CalButton settingsButton;

    void resetConfig();

    struct ConfigVars {
        int headerIndexSize;
        int titleSize;

        int marginNarrow;
        int marginWide;

        int marginLeft;
        int marginTop;
        int marginRight;
        int marginBottom;

        Color defaultColor;
        Color weekendColor;
        Color monthColor;
        Color curDayColor;

        int numberSize;
        FontInfo defaultFont;

        float columnSizes[7];
        int rowAmount;
    }_config;
};


#endif // DESKCAL_H
