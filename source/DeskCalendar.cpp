#include "DeskCalendar.h"
#include "TextUtils.h"

#include <cstdio>
#include <ctime>
#include <cmath>
#include <algorithm>

bool DeskCalendar::loadConfig()
{
    resetConfig();

    std::ifstream infile("deskcal.cfg");
    if (!infile.is_open()) {
        return false;
    }

    std::string lineStr;
    while (std::getline(infile, lineStr)) {
        std::stringstream line(lineStr);
        std::string input;

        if (line >> input) {
            // Ignoring comments
            if (input.substr(0, 2) == "//") {
                continue;
            }

            char c;
            if (!(line >> c) || c != '=') {
                // Ignoring invalid configuration line
                continue;
            }

            if (input == "headerIndexSize") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _config.headerIndexSize = newVal;
            } else if (input == "titleSize") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _config.titleSize = newVal;
            } else if (input == "marginNarrow") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _config.marginNarrow = newVal;
            } else if (input == "marginWide") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _config.marginWide = newVal;
            } else if (input == "marginLeft") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _config.marginLeft = newVal;
            } else if (input == "marginTop") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _config.marginTop = newVal;
            } else if (input == "marginRight") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _config.marginRight = newVal;
            } else if (input == "marginBottom") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _config.marginBottom = newVal;
            } else if (input == "numberSize") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _config.numberSize = newVal;
            } else if (input == "defaultColor") {
                line >> _config.defaultColor;
            } else if (input == "weekendColor") {
                line >> _config.weekendColor;
            } else if (input == "monthColor") {
                line >> _config.monthColor;
            } else if (input == "curDayColor") {
                line >> _config.curDayColor;
            } else if (input == "defaultFont") {
                line >> _config.defaultFont;
            } else if (input == "columnSizes") {
                float sizes[7];
                if (!(line >> c >> sizes[0] >> c >> sizes[1] >> c >> sizes[2] >> c >> sizes[3] >> c >> sizes[4] >> c >> sizes[5] >> c >> sizes[6] >> c)) {
                    continue;
                }
                for (int i = 0; i < 7; ++i) {
                    _config.columnSizes[i] = sizes[i];
                }
            } else if (input == "rowAmount") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _config.rowAmount = newVal;
            }
        }
    }
    return true;
}

bool DeskCalendar::saveConfig() const
{
    std::ofstream outfile("deskcal.cfg.tmp");
    if (!outfile.is_open()) {
        return false;
    }

    outfile << "// Height of Day headers and width of Week indices" << std::endl;
    outfile << "headerIndexSize = " << _config.headerIndexSize << std::endl;
    outfile << "// Height of Title-bar" << std::endl;
    outfile << "titleSize = " << _config.titleSize << std::endl;
    outfile << std::endl;
    outfile << "// Space between calendar cells" << std::endl;
    outfile << "marginNarrow = " << _config.marginNarrow << std::endl;
    outfile << "// Space between calendar cell area and headers/indices, also between headers and title-bar" << std::endl;
    outfile << "marginWide = " << _config.marginWide << std::endl;
    outfile << std::endl;
    outfile << "// Space between the left side of the screen and the window" << std::endl;
    outfile << "marginLeft = " << _config.marginLeft << std::endl;
    outfile << "// Space between the top side of the screen and the window" << std::endl;
    outfile << "marginTop = " << _config.marginTop << std::endl;
    outfile << "// Space between the right side of the screen and the window" << std::endl;
    outfile << "marginRight = " << _config.marginRight << std::endl;
    outfile << "// Space between the bottom side of the screen (not including task-bar) and the window" << std::endl;
    outfile << "marginBottom = " << _config.marginBottom << std::endl;
    outfile << std::endl;
    outfile << "// (Red[0, 255], Green[0, 255], Blue[0, 255], Alpha[0, 255])" << std::endl;
    outfile << "// Default color of all blocks, except weekend cells and month headers" << std::endl;
    outfile << "defaultColor = " << _config.defaultColor << std::endl;
    outfile << "// Default color of all weekend cells" << std::endl;
    outfile << "weekendColor = " << _config.weekendColor << std::endl;
    outfile << "// Color of all month headers" << std::endl;
    outfile << "monthColor = " << _config.monthColor << std::endl;
    outfile << "// Current day highlight color" << std::endl;
    outfile << "curDayColor = " << _config.curDayColor << std::endl;
    outfile << std::endl;
    outfile << "// Default font of all blocks, except cell numbers and indices" << std::endl;
    outfile << "// (TypeFace, Size, Weight/Boldness[1, 1000], isItalic[0, 1], isUnderlined[0, 1], isStrikeout[0, 1])" << std::endl;
    outfile << "defaultFont = " << _config.defaultFont << std::endl;
    outfile << "// Font size for cell and index numbers" << std::endl;
    outfile << "numberSize = " << _config.numberSize << std::endl;
    outfile << std::endl;
    outfile << "// Proportional widths for all 7 columns, must add up to 1" << std::endl;
    outfile << "columnSizes = " << '(' << _config.columnSizes[0] << ", " << _config.columnSizes[1] << ", " << _config.columnSizes[2] << ", "
                                       << _config.columnSizes[3] << ", " << _config.columnSizes[4] << ", " << _config.columnSizes[5] << ", "
                                       << _config.columnSizes[6] << ')' << std::endl;
    outfile << "// Maximum amount of rows on screen" << std::endl;
    outfile << "rowAmount = " << _config.rowAmount << std::endl;

    outfile.close();
    remove("deskcal.cfg");
    rename("deskcal.cfg.tmp", "deskcal.cfg");

    return true;
}

void DeskCalendar::resetConfig()
{
    _config.headerIndexSize = 24;
    _config.titleSize = 40;

    _config.marginNarrow = 2;
    _config.marginWide = 6;

    _config.marginLeft = 100;
    _config.marginTop = 50;
    _config.marginRight = 50;
    _config.marginBottom = 50;

    _config.defaultColor = Color(0, 0, 255, 128);
    _config.weekendColor = Color(255, 192, 0, 128);
    _config.monthColor = Color(255, 0, 0, 128);
    _config.curDayColor = Color(255, 0, 0, 255);

    _config.defaultFont = FontInfo{"Times New Roman", 18, 400, 0, 0, 0};
    _config.numberSize = 20;

    for (int i = 0; i < 7; ++i) {
        _config.columnSizes[i] = 0.142857;
    }
    _config.rowAmount = 5;
}

bool DeskCalendar::loadDates()
{
    std::ifstream infile("deskcal.sav");
    if (!infile.is_open()) {
        return false;
    }
    _editedDates.clear();

    std::string lineStr;
    while (std::getline(infile, lineStr)) {
        CalDate newCalDate;
        std::stringstream line(lineStr);
        if (!(line >> newCalDate)) {
            continue;
        }

        _editedDates.push_back(newCalDate);
    }

    std::sort(_editedDates.begin(), _editedDates.end());
    return true;
}

bool DeskCalendar::saveDates() const
{
    std::ofstream outfile("deskcal.sav.tmp");
    if (!outfile.is_open()) {
        return false;
    }

    for (auto calDate : _editedDates) {
        outfile << calDate << std::endl;
    }

    outfile.close();
    remove("deskcal.sav");
    rename("deskcal.sav.tmp", "deskcal.sav");

    return true;
}

void DeskCalendar::update()
{
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    workArea.left += _config.marginLeft;
    workArea.top += _config.marginTop;
    workArea.right -= _config.marginRight;
    workArea.bottom -= _config.marginBottom;

    //int screenX = workArea.left;
    //int screenY = workArea.top;
    int screenW = workArea.right - workArea.left;
    int screenH = workArea.bottom - workArea.top;
    //SetWindowPos(_hwnd, HWND_NOTOPMOST, screenX, screenY, screenW, screenH, SWP_SHOWWINDOW);

    const wchar_t* days[7] = {
        L"Δευτέρα", L"Τρίτη", L"Τετάρτη", L"Πέμπτη", L"Παρασκευή", L"Σάββατο", L"Κυριακή"
    };
    const wchar_t* months[2][12] = {
        { L"Ιανουάριος", L"Φεβρουάριος", L"Μάρτιος", L"Απρίλιος", L"Μάιος", L"Ιούνιος",
          L"Ιούλιος", L"Αύγουστος", L"Σεπτέμβριος", L"Οκτώβριος", L"Νοέμβριος", L"Δεκέμβριος" },
        { L"Ιανουαρίου", L"Φεβρουαρίου", L"Μαρτίου", L"Απριλίου", L"Μαΐου", L"Ιουνίου",
          L"Ιουλίου", L"Αυγούστου", L"Σεπτεμβρίου", L"Οκτωβρίου", L"Νοεμβρίου", L"Δεκεμβρίου" }
    };

    time_t curTime = time(0);
    tm timeInfo = *localtime(&curTime);
    CalDate::Date today(timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday);

    int wDay = timeInfo.tm_wday - 1;
    if (wDay < 0) {
        wDay += 7;
    }
    std::wstring toDate = std::wstring(L"Σήμερα είναι ") + std::to_wstring(timeInfo.tm_mday) + L" " + months[1][timeInfo.tm_mon] + L" "
                        + std::to_wstring(timeInfo.tm_year + 1900) + L", " + days[wDay];

    int titleX = _config.headerIndexSize + _config.marginWide;
    int titleY = 0;
    int titleW = screenW - _config.headerIndexSize - _config.marginWide;
    int titleH = _config.titleSize;

    closeButton.update(titleX + titleW - titleH - 5, titleY, titleH, titleH);
    nextButton.update(titleX + titleW - titleH*4, titleY, titleH, titleH);
    todayButton.update(titleX + titleW - titleH*6, titleY, titleH, titleH);
    prevButton.update(titleX + titleW - titleH*8, titleY, titleH, titleH);
    settingsButton.update(titleX, titleY, titleH, titleH);

    _renderedTitle = CalTitle(toDate, titleX, titleY, titleW - _config.marginNarrow, titleH);

    _renderedHeaders.clear();
    float Y = titleH + _config.marginWide, X = _config.headerIndexSize + _config.marginWide;
    for (int i = 0; i < 7; ++i) {
        int boxH = _config.headerIndexSize;
        float boxW = titleW*_config.columnSizes[i] - _config.marginNarrow;

        _renderedHeaders.push_back(CalHeader(days[i], std::round(X), std::round(Y), std::round(boxW), boxH, _config.defaultColor));
        X += boxW + _config.marginNarrow;
    }
    Y += _config.headerIndexSize + _config.marginWide;

    tm tmpTM = {
        0, 0, 0, 1, 0, timeInfo.tm_year, 0, 0, 0
    };
    time_t tmpTime = mktime(&tmpTM);
    tmpTM = *localtime(&tmpTime);

    int curWeek = tmpTM.tm_wday - 1;
    if (curWeek < 0) {
        curWeek += 7;
    }

    tmpTM = {
        0, 0, 0, _curDate.day, _curDate.month - 1, _curDate.year - 1900, 0, 0, 0
    };
    tmpTime = mktime(&tmpTM);
    tmpTM = *localtime(&tmpTime);

    curWeek = (curWeek + tmpTM.tm_yday)/7;

    _renderedIndices.clear();
    for (int i = 0; i < _config.rowAmount; ++i) {
        float boxH = (screenH - titleH - _config.headerIndexSize - 2*_config.marginWide)/(float)_config.rowAmount - _config.marginNarrow;

        _renderedIndices.push_back(CalIndex((curWeek + i) % 52 + 1, 0, std::round(Y), _config.headerIndexSize, boxH));
        Y += boxH + _config.marginNarrow;
    }
    Y = titleH + _config.marginWide + _config.headerIndexSize + _config.marginWide;

    _dummyDates.clear();
    _renderedDates.clear();

    tm firstDay = {
        0, 0, 0, _curDate.day, _curDate.month - 1, _curDate.year - 1900, 0, 0, 0
    };
    time_t firstTime = mktime(&firstDay);
    firstDay = *localtime(&firstTime);
    wDay = firstDay.tm_wday - 1;
    if (wDay < 0) {
        wDay += 7;
    }

    curTime = firstTime - wDay*24*3600;
    for (int i = 0; i < _config.rowAmount; ++i) {
        float boxH = (screenH - titleH - _config.headerIndexSize - 2*_config.marginWide)/(float)_config.rowAmount - _config.marginNarrow;
        X = _config.headerIndexSize + _config.marginWide;
        for (int j = 0; j < 7; ++j) {
            tm curDay = *localtime(&curTime);
            CalDate::Date date(curDay.tm_year + 1900, curDay.tm_mon + 1, curDay.tm_mday);
            float boxW = titleW*_config.columnSizes[j] - _config.marginNarrow;
            Color boxColor = _config.defaultColor;
            if (curDay.tm_wday == 0 || curDay.tm_wday == 6) {
                boxColor = _config.weekendColor;
            }

            auto it = std::lower_bound(_editedDates.begin(), _editedDates.end(), date);
            if (it != _editedDates.end() && it->date == date) {
                if (date.day == 1) {
                    _renderedHeaders.emplace_back(std::wstring(months[0][date.month - 1]) + L" " + std::to_wstring(date.year),
                                                  X, Y, boxW, _config.headerIndexSize, _config.monthColor);
                    _renderedDates.emplace_back(X, Y + _config.headerIndexSize + _config.marginNarrow, boxW,
                                                boxH - _config.headerIndexSize - _config.marginNarrow, date, &_editedDates);
                } else {
                    _renderedDates.emplace_back(X, Y, boxW, boxH, date, &_editedDates);
                }
            } else {
                if (date.day == 1) {
                    _renderedHeaders.emplace_back(std::wstring(months[0][date.month - 1]) + L" " + std::to_wstring(date.year),
                                                  X, Y, boxW, _config.headerIndexSize, _config.monthColor);
                    _dummyDates.emplace_back(date, L"", boxColor, _config.defaultFont);
                    _renderedDates.emplace_back(X, Y + _config.headerIndexSize + _config.marginNarrow, boxW,
                                                boxH - _config.headerIndexSize - _config.marginNarrow, date, &_dummyDates);
                } else {
                    _dummyDates.emplace_back(date, L"", boxColor, _config.defaultFont);
                    _renderedDates.emplace_back(X, Y, boxW, boxH, date, &_dummyDates);
                }
            }

            X += boxW + _config.marginNarrow;
            curTime += 24*3600;
        }
        Y += boxH + _config.marginNarrow;
    }
}

void DeskCalendar::render()
{
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    workArea.left += _config.marginLeft;
    workArea.top += _config.marginTop;
    workArea.right -= _config.marginRight;
    workArea.bottom -= _config.marginBottom;

    int screenX = workArea.left;
    int screenY = workArea.top;
    int screenW = workArea.right - workArea.left;
    int screenH = workArea.bottom - workArea.top;

    RECT wndRect;
    GetClientRect(_hwnd, &wndRect);

    if (wndRect.left != workArea.left || wndRect.right != workArea.right || wndRect.top != workArea.top || wndRect.bottom != workArea.bottom) {
       SetWindowPos(_hwnd, HWND_NOTOPMOST, screenX, screenY, screenW, screenH, SWP_SHOWWINDOW);
    }

    time_t now = time(0);
    tm tmNow = *localtime(&now);
    CalDate::Date today(tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday);

    WBitmap canvas(screenW, screenH, Color());

    _renderedTitle.renderGraphics(canvas, _config.defaultColor);
    closeButton.renderOnBmp(canvas);
    nextButton.renderOnBmp(canvas);
    prevButton.renderOnBmp(canvas);
    todayButton.renderOnBmp(canvas);
    settingsButton.renderOnBmp(canvas);

    for (CalHeader& header : _renderedHeaders) {
        header.renderGraphics(canvas);
    }

    for (CalIndex& index : _renderedIndices) {
        index.renderGraphics(canvas, _config.defaultColor);
    }
    for (DatePointer& date : _renderedDates) {
        auto it = std::lower_bound(date.ptr->begin(), date.ptr->end(), date.date);
        if (it != date.ptr->end() && it->date == date.date) {
            if (date.date == today) {
                WBitmap frame(date.w + _config.marginNarrow*2, date.h + _config.marginNarrow*2, _config.curDayColor);
                WBitmap hollow(date.w, date.h, Color());

                hollow.renderOnBmp(frame, _config.marginNarrow, _config.marginNarrow, false);
                frame.renderOnBmp(canvas, date.x - _config.marginNarrow, date.y - _config.marginNarrow);
            }
            it->renderGraphics(canvas, date.x, date.y, date.w, date.h);
        }
    }
    canvas.renderOnWnd(_hwnd);

    _renderedTitle.renderText(_hwnd, _config.defaultFont);
    for (CalHeader& header : _renderedHeaders) {
        header.renderText(_hwnd, _config.defaultFont);
    }
    for (CalIndex& index : _renderedIndices) {
        index.renderText(_hwnd, _config.numberSize);
    }
    for (DatePointer& date : _renderedDates) {
        auto it = std::lower_bound(date.ptr->begin(), date.ptr->end(), date.date);
        if (it != date.ptr->end() && it->date == date.date) {
            it->renderText(_hwnd, date.x, date.y, date.w, date.h, _config.numberSize);
        }
    }
}

void DeskCalendar::onClickNext()
{
    tm curTM = {
        0, 0, 0, _curDate.day, _curDate.month - 1, _curDate.year - 1900, 0, 0, 0
    };
    time_t curTime = mktime(&curTM);

    curTime += 7*24*3600;
    curTM = *localtime(&curTime);

    setCurrentDate(CalDate::Date(curTM.tm_year + 1900, curTM.tm_mon + 1, curTM.tm_mday));
}

void DeskCalendar::onClickToday()
{
    time_t now = time(0);
    tm today = *localtime(&now);

    setCurrentDate(CalDate::Date(today.tm_year + 1900, today.tm_mon + 1, today.tm_mday));
}

void DeskCalendar::onClickPrev()
{
    tm curTM = {
        0, 0, 0, _curDate.day, _curDate.month - 1, _curDate.year - 1900, 0, 0, 0
    };
    time_t curTime = mktime(&curTM);

    curTime -= 7*24*3600;
    curTM = *localtime(&curTime);

    setCurrentDate(CalDate::Date(curTM.tm_year + 1900, curTM.tm_mon + 1, curTM.tm_mday));
}

void DeskCalendar::onClickSettings()
{

}