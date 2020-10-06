#include "DeskCalendar.h"
#include "TextUtils.h"
#include "../resource.h"

#include <cstdio>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <windowsx.h>
#include <commctrl.h>


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
        _config.columnSizes[i] = 1.0f/7.0f;
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

    for (CalDate& date : _dummyDates) {
        if (date.getText() != L"" || date.getFont() != nullptr || date.getColor() != nullptr) {
            auto it = std::lower_bound(_editedDates.begin(), _editedDates.end(), date);
            _editedDates.insert(it, date);
            _edited = true;
        }
    }
    if (_edited) {
        saveDates();
    }

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
        boxW = (boxW < 0.0f) ? 0.0f : boxW;

        _renderedHeaders.push_back(CalHeader(days[i], X, Y, boxW, boxH, _config.defaultColor));
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
        boxH = (boxH < 0.0f) ? 0.0f : boxH;

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
        boxH = (boxH < 0.0f) ? 0.0f : boxH;
        X = _config.headerIndexSize + _config.marginWide;
        for (int j = 0; j < 7; ++j) {
            tm curDay = *localtime(&curTime);
            CalDate::Date date(curDay.tm_year + 1900, curDay.tm_mon + 1, curDay.tm_mday);
            float boxW = titleW*_config.columnSizes[j] - _config.marginNarrow;
            boxW = (boxW < 0.0f) ? 0.0f : boxW;
            Color boxColor = _config.defaultColor;
            if (curDay.tm_wday == 0 || curDay.tm_wday == 6) {
                boxColor = _config.weekendColor;
            }

            auto it = std::lower_bound(_editedDates.begin(), _editedDates.end(), date);
            if (it != _editedDates.end() && it->date == date) {
                if (date.day == 1) {
                    _renderedHeaders.emplace_back(std::wstring(months[0][date.month - 1]) + L" " + std::to_wstring(date.year),
                                                  X, Y, boxW, _config.headerIndexSize, _config.monthColor);

                    int sizeH = boxH - _config.headerIndexSize - _config.marginNarrow;
                    sizeH = (sizeH < 0) ? 0 : sizeH;
                    _renderedDates.emplace_back(X, Y + _config.headerIndexSize + _config.marginNarrow,
                                                boxW, sizeH, date, &_editedDates);
                } else {
                    _renderedDates.emplace_back(X, Y, boxW, boxH, date, &_editedDates);
                }
            } else {
                if (date.day == 1) {
                    _renderedHeaders.emplace_back(std::wstring(months[0][date.month - 1]) + L" " + std::to_wstring(date.year),
                                                  X, Y, boxW, _config.headerIndexSize, _config.monthColor);
                    _dummyDates.emplace_back(date, L"");

                    int sizeH = boxH - _config.headerIndexSize - _config.marginNarrow;
                    sizeH = (sizeH < 0) ? 0 : sizeH;
                    _renderedDates.emplace_back(X, Y + _config.headerIndexSize + _config.marginNarrow,
                                                boxW, sizeH, date, &_dummyDates);
                } else {
                    _dummyDates.emplace_back(date, L"");
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

            it->renderGraphics(canvas, date.x, date.y, date.w, date.h, _config.defaultColor, _config.weekendColor);
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
            it->renderText(_hwnd, date.x, date.y, date.w, date.h, _config.numberSize, _config.defaultFont);
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

WNDPROC defEditProc;
LRESULT CALLBACK editProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void DeskCalendar::onClick(int x, int y)
{
    if (_selected) {
        int textLen = Edit_GetTextLength(_editWnd);
        std::wstring editText(textLen, 0);

        Edit_GetText(_editWnd, &editText[0], textLen + 1);

        auto it = std::lower_bound(_selected->ptr->begin(), _selected->ptr->end(), _selected->date);
        if (it != _selected->ptr->end() && it->date == _selected->date) {
            it->renderGraphics(_hwnd, _selected->x, _selected->y, _selected->w, _selected->h, _config.defaultColor, _config.weekendColor);
            if (editText != it->getText()) {
                it->setText(editText);
                _edited = true;
            }
            it->renderText(_hwnd, _selected->x, _selected->y, _selected->w, _selected->h, _config.numberSize, _config.defaultFont);

            tm curTM = {
                0, 0, 0, it->date.day, it->date.month - 1, it->date.year - 1900, 0, 0, 0
            };
            time_t curTime = mktime(&curTM);
            curTM = *localtime(&curTime);

            if (curTM.tm_wday == 0 || curTM.tm_wday == 6) {
                cellButton.renderOnWnd(_hwnd, it->getColor() != nullptr ? *(it->getColor()) : _config.weekendColor);
            } else {
                cellButton.renderOnWnd(_hwnd, it->getColor() != nullptr ? *(it->getColor()) : _config.defaultColor);
            }
        }

        DestroyWindow(_editWnd);
        DeleteObject(_editFont);
        _selected = nullptr;
    }

    for (DatePointer& ptr : _renderedDates) {
        if (ptr.x <= x && ptr.y <= y && x < ptr.x + ptr.w && y < ptr.y + ptr.h) {
            _selected = &ptr;
            break;
        }
    }

    if (_selected) {
        auto it = std::lower_bound(_selected->ptr->begin(), _selected->ptr->end(), _selected->date);
        if (it != _selected->ptr->end() && it->date == _selected->date) {
            _editWnd = CreateWindowEx(0, L"EDIT", L"", WS_VISIBLE | ES_LEFT | ES_AUTOVSCROLL | ES_MULTILINE | WS_POPUP,
                                    _selected->x + _config.marginLeft, _selected->y + _config.numberSize + _config.marginTop, _selected->w, _selected->h - _config.numberSize,
                                    _hwnd, 0, GetModuleHandle(0), NULL);
            defEditProc = (WNDPROC)SetWindowLongPtr(_editWnd, GWLP_WNDPROC, (LONG_PTR)editProc);
            SetFocus(_editWnd);

            _editFont = it->createFont(_config.defaultFont);
            SendMessage(_editWnd, WM_SETFONT, WPARAM(_editFont), TRUE);

            Edit_SetText(_editWnd, it->getText().c_str());
        }
    }
}

void DeskCalendar::onHover(int x, int y)
{
    static DatePointer* prevHover = nullptr;

    auto it = _renderedDates.begin();
    for (; it < _renderedDates.end(); ++it) {
        if (it->x <= x && it->y <= y && x < it->x + it->w && y < it->y + it->h) {
            break;
        }
    }

    if (it != _renderedDates.end() && prevHover != &*it) {
        auto dateIt = std::lower_bound(it->ptr->begin(), it->ptr->end(), it->date);
        if (dateIt != it->ptr->end() && dateIt->date == it->date) {
            if (cellButtonVisible) {
                cellButton.maskOnWnd(_hwnd);
            }
            cellButton.update(it->x + it->w - _config.numberSize, it->y, _config.numberSize, _config.numberSize);

            tm curTM = {
                0, 0, 0, dateIt->date.day, dateIt->date.month - 1, dateIt->date.year - 1900, 0, 0, 0
            };
            time_t curTime = mktime(&curTM);
            curTM = *localtime(&curTime);

            if (curTM.tm_wday == 0 || curTM.tm_wday == 6) {
                cellButton.renderOnWnd(_hwnd, dateIt->getColor() != nullptr ? *(dateIt->getColor()) : _config.weekendColor);
            } else {
                cellButton.renderOnWnd(_hwnd, dateIt->getColor() != nullptr ? *(dateIt->getColor()) : _config.defaultColor);
            }
            cellButtonVisible = true;

            prevHover = &*it;
        }
    } else if (it == _renderedDates.end()) {
        if (cellButtonVisible) {
            cellButton.maskOnWnd(_hwnd);
        }
        cellButtonVisible = false;
        prevHover = nullptr;
    }
}

std::vector<std::wstring> g_typefaces;
DeskCalendar *g_deskcal;
DeskCalendar::ConfigVars g_config, old_config;

const std::vector<std::wstring> enumerateFonts();
INT_PTR settingsDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void DeskCalendar::onClickSettings()
{
    g_deskcal = this;
    g_config = old_config = _config;
    g_typefaces = enumerateFonts();
    INT_PTR result = DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_SETTINGS), _hwnd, settingsDlgProc);
    if (result == IDOK) {
        _config = g_config;
        saveConfig();
    }
}

HWND g_hwnd;
CalDate g_setDate;
DeskCalendar::DatePointer g_setDatePtr;
int g_numSize;

INT_PTR dateSettingsDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void DeskCalendar::onClickCell(int x, int y)
{
    auto it = _renderedDates.begin();
    for (; it < _renderedDates.end(); ++it) {
        if (it->x <= x && it->y <= y && x < it->x + it->w && y < it->y + it->h) {
            break;
        }
    }

    if (it != _renderedDates.end()) {
        auto dateIt = std::lower_bound(it->ptr->begin(), it->ptr->end(), it->date);
        if (dateIt != it->ptr->end() && dateIt->date == it->date) {
            g_hwnd = _hwnd;
            g_setDate = *dateIt;
            g_setDatePtr = *it;
            g_numSize = _config.numberSize;
            g_config = old_config = _config;

            g_typefaces = enumerateFonts();
            INT_PTR result = DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DATE_SETTINGS), _hwnd, dateSettingsDlgProc);
            if (result == IDOK) {
                *dateIt = g_setDate;
                _edited = true;

                tm curTM = {
                    0, 0, 0, g_setDate.date.day, g_setDate.date.month - 1, g_setDate.date.year - 1900, 0, 0, 0
                };
                time_t curTime = mktime(&curTM);
                curTM = *localtime(&curTime);

                if (curTM.tm_wday == 0 || curTM.tm_wday == 6) {
                    cellButton.renderOnWnd(_hwnd, g_setDate.getColor() != nullptr ? *(g_setDate.getColor()) : _config.weekendColor);
                } else {
                    cellButton.renderOnWnd(_hwnd, g_setDate.getColor() != nullptr ? *(g_setDate.getColor()) : _config.defaultColor);
                }
            }
        }
    }
}

LRESULT CALLBACK editProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_ESCAPE:
            SendMessage(GetParent(hwnd), WM_LBUTTONDOWN, 0, 0);
            return 0;
        }
    break;
    }
    return CallWindowProc(defEditProc, hwnd, msg, wParam, lParam);
}

bool retrieveDateSettings(HWND hwnd);
HWND CreateToolTip(int toolID, HWND hDlg, const wchar_t* pszText);

INT_PTR dateSettingsDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static std::vector<HWND> tooltips;

    switch (msg)
    {
    case WM_INITDIALOG:
    {
        FontInfo font = g_setDate.getFont() != nullptr ? *(g_setDate.getFont()) : g_config.defaultFont;
        Color color;

        tm curTM = {
            0, 0, 0, g_setDate.date.day, g_setDate.date.month - 1, g_setDate.date.year - 1900, 0, 0, 0
        };
        time_t curTime = mktime(&curTM);
        curTM = *localtime(&curTime);

        if (curTM.tm_wday == 0 || curTM.tm_wday == 6) {
            color = g_setDate.getColor() != nullptr ? *(g_setDate.getColor()) : g_config.weekendColor;
        } else {
            color = g_setDate.getColor() != nullptr ? *(g_setDate.getColor()) : g_config.defaultColor;
        }

        bool listed = g_setDate.getListed();
        const wchar_t* months[12] = {
            L" Ιανουαρίου ", L" Φεβρουαρίου ", L" Μαρτίου ", L" Απριλίου ", L" Μαΐου ", L" Ιουνίου ",
            L" Ιουλίου ", L" Αυγούστου ", L" Σεπτεμβρίου ", L" Οκτωβρίου ", L" Νοεμβρίου ", L" Δεκεμβρίου "
        };

        std::wstring title = std::wstring(L"Επιλογές για ") +
                             std::to_wstring(g_setDate.date.day) +
                             months[g_setDate.date.month - 1] +
                             std::to_wstring(g_setDate.date.year);
        SetWindowText(hwnd, title.c_str());

        for (auto tf : g_typefaces) {
            ComboBox_AddString(GetDlgItem(hwnd, ID_FONT_NAME), tf.c_str());
        }
        ComboBox_SetText(GetDlgItem(hwnd, ID_FONT_NAME), utf8_to_utf16(font.typeface).c_str());

        tooltips.push_back(CreateToolTip(ID_FONT_SIZE, hwnd, L"Ύψος χαρακτήρων"));
        Edit_SetText(GetDlgItem(hwnd, ID_FONT_SIZE), std::to_wstring(font.size).c_str());
        tooltips.push_back(CreateToolTip(ID_FONT_WEIGHT, hwnd, L"Από 1 μέχρι και 1000"));
        Edit_SetText(GetDlgItem(hwnd, ID_FONT_WEIGHT), std::to_wstring(font.weight).c_str());

        CheckDlgButton(hwnd, IDCK_ITALIC, font.italic ? BST_CHECKED : BST_UNCHECKED);
        CheckDlgButton(hwnd, IDCK_UNDERLINE, font.underlined ? BST_CHECKED : BST_UNCHECKED);
        CheckDlgButton(hwnd, IDCK_LIST, listed ? BST_CHECKED : BST_UNCHECKED);
        CheckDlgButton(hwnd, IDCK_STRIKEOUT, font.strikeout ? BST_CHECKED : BST_UNCHECKED);

        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_RED), std::to_wstring(color.r).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_GREEN), std::to_wstring(color.g).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_BLUE), std::to_wstring(color.b).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_ALPHA), std::to_wstring(color.a).c_str());

        return TRUE;
    }
    case WM_COMMAND:
        switch (wParam)
        {
        case IDOK:
            if (retrieveDateSettings(hwnd)) {
                g_setDate.renderGraphics(g_hwnd, g_setDatePtr.x, g_setDatePtr.y, g_setDatePtr.w, g_setDatePtr.h, g_config.defaultColor, g_config.weekendColor);
                g_setDate.renderText(g_hwnd, g_setDatePtr.x, g_setDatePtr.y, g_setDatePtr.w, g_setDatePtr.h, g_numSize, g_config.defaultFont);
                EndDialog(hwnd, IDOK);
            }
            return TRUE;
        case IDB_APPLY:
            if (retrieveDateSettings(hwnd)) {
                g_setDate.renderGraphics(g_hwnd, g_setDatePtr.x, g_setDatePtr.y, g_setDatePtr.w, g_setDatePtr.h, g_config.defaultColor, g_config.weekendColor);
                g_setDate.renderText(g_hwnd, g_setDatePtr.x, g_setDatePtr.y, g_setDatePtr.w, g_setDatePtr.h, g_numSize, g_config.defaultFont);
            }
            return TRUE;
        case IDCANCEL:
            g_setDatePtr.renderGraphics(g_hwnd, old_config.defaultColor, old_config.weekendColor);
            g_setDatePtr.renderText(g_hwnd, g_numSize, old_config.defaultFont);
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
    break;
    case WM_CLOSE:
        for (HWND toolWnd : tooltips) {
            DestroyWindow(toolWnd);
        }
        tooltips.clear();
        g_setDatePtr.renderGraphics(g_hwnd, old_config.defaultColor, old_config.weekendColor);
        g_setDatePtr.renderText(g_hwnd, g_numSize, old_config.defaultFont);
        EndDialog(hwnd, IDCANCEL);
        return TRUE;
    }
    return FALSE;
}

bool retrieveSettings(HWND hwnd);

INT_PTR settingsDlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static std::vector<HWND> tooltips;

    switch (msg)
    {
    case WM_INITDIALOG:
    {
        FontInfo font = g_config.defaultFont;
        for (auto tf : g_typefaces) {
            ComboBox_AddString(GetDlgItem(hwnd, ID_FONT_NAME), tf.c_str());
        }
        ComboBox_SetText(GetDlgItem(hwnd, ID_FONT_NAME), utf8_to_utf16(font.typeface).c_str());

        tooltips.push_back(CreateToolTip(ID_FONT_SIZE, hwnd, L"Ύψος χαρακτήρων"));
        Edit_SetText(GetDlgItem(hwnd, ID_FONT_SIZE), std::to_wstring(font.size).c_str());
        tooltips.push_back(CreateToolTip(ID_FONT_WEIGHT, hwnd, L"Από 1 μέχρι και 1000"));
        Edit_SetText(GetDlgItem(hwnd, ID_FONT_WEIGHT), std::to_wstring(font.weight).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_NUMSIZE), std::to_wstring(g_config.numberSize).c_str());

        CheckDlgButton(hwnd, IDCK_ITALIC, font.italic ? BST_CHECKED : BST_UNCHECKED);
        CheckDlgButton(hwnd, IDCK_UNDERLINE, font.underlined ? BST_CHECKED : BST_UNCHECKED);
        CheckDlgButton(hwnd, IDCK_STRIKEOUT, font.strikeout ? BST_CHECKED : BST_UNCHECKED);

        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_RED_DEFAULT), std::to_wstring(g_config.defaultColor.r).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_GREEN_DEFAULT), std::to_wstring(g_config.defaultColor.g).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_BLUE_DEFAULT), std::to_wstring(g_config.defaultColor.b).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_ALPHA_DEFAULT), std::to_wstring(g_config.defaultColor.a).c_str());

        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_RED_WEEKEND), std::to_wstring(g_config.weekendColor.r).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_GREEN_WEEKEND), std::to_wstring(g_config.weekendColor.g).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_BLUE_WEEKEND), std::to_wstring(g_config.weekendColor.b).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_ALPHA_WEEKEND), std::to_wstring(g_config.weekendColor.a).c_str());

        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_RED_MONTH), std::to_wstring(g_config.monthColor.r).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_GREEN_MONTH), std::to_wstring(g_config.monthColor.g).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_BLUE_MONTH), std::to_wstring(g_config.monthColor.b).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_ALPHA_MONTH), std::to_wstring(g_config.monthColor.a).c_str());

        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_RED_DAY), std::to_wstring(g_config.curDayColor.r).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_GREEN_DAY), std::to_wstring(g_config.curDayColor.g).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_BLUE_DAY), std::to_wstring(g_config.curDayColor.b).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_COLOR_ALPHA_DAY), std::to_wstring(g_config.curDayColor.a).c_str());

        Edit_SetText(GetDlgItem(hwnd, ID_MARGIN_NARROW), std::to_wstring(g_config.marginNarrow).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_MARGIN_WIDE), std::to_wstring(g_config.marginWide).c_str());

        Edit_SetText(GetDlgItem(hwnd, ID_MARGIN_LEFT), std::to_wstring(g_config.marginLeft).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_MARGIN_TOP), std::to_wstring(g_config.marginTop).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_MARGIN_RIGHT), std::to_wstring(g_config.marginRight).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_MARGIN_BOTTOM), std::to_wstring(g_config.marginBottom).c_str());

        Edit_SetText(GetDlgItem(hwnd, ID_TITLE_SIZE), std::to_wstring(g_config.titleSize).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_HEADER_SIZE), std::to_wstring(g_config.headerIndexSize).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_WEEKS), std::to_wstring(g_config.rowAmount).c_str());

        Edit_SetText(GetDlgItem(hwnd, ID_PERCENT_MON), std::to_wstring(g_config.columnSizes[0]*100).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_PERCENT_TUE), std::to_wstring(g_config.columnSizes[1]*100).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_PERCENT_WED), std::to_wstring(g_config.columnSizes[2]*100).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_PERCENT_THU), std::to_wstring(g_config.columnSizes[3]*100).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_PERCENT_FRI), std::to_wstring(g_config.columnSizes[4]*100).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_PERCENT_SAT), std::to_wstring(g_config.columnSizes[5]*100).c_str());
        Edit_SetText(GetDlgItem(hwnd, ID_PERCENT_SUN), std::to_wstring(g_config.columnSizes[6]*100).c_str());

        return TRUE;
    }
    case WM_COMMAND:
        switch (wParam)
        {
        case IDOK:
            if (retrieveSettings(hwnd)) {
                g_deskcal->setConfig(g_config);
                g_deskcal->update();
                g_deskcal->render();
                EndDialog(hwnd, IDOK);
            }
            return TRUE;
        case IDB_APPLY:
            if (retrieveSettings(hwnd)) {
                DeskCalendar::ConfigVars oldConfig = g_deskcal->getConfig();
                g_deskcal->setConfig(g_config);
                g_deskcal->update();
                g_deskcal->render();
                g_deskcal->setConfig(oldConfig);
            }
            return TRUE;
        case IDCANCEL:
            g_deskcal->update();
            g_deskcal->render();
            EndDialog(hwnd, IDCANCEL);
            return TRUE;
        }
    break;
    case WM_CLOSE:
        for (HWND toolWnd : tooltips) {
            DestroyWindow(toolWnd);
        }
        tooltips.clear();
        g_deskcal->update();
        g_deskcal->render();
        EndDialog(hwnd, IDCANCEL);
        return TRUE;
    }
    return FALSE;
}

int CALLBACK EnumFontFamExProc(const LOGFONT *lpelfe, const TEXTMETRIC *lpntme, DWORD FontType, LPARAM lParam);

const std::vector<std::wstring> enumerateFonts()
{
    std::vector<std::wstring> typefaces;
    HDC hdc = GetDC(0);

    LOGFONT lf = { 0 };
    lf.lfCharSet = GREEK_CHARSET;
    lf.lfFaceName[0] = '\0';

    EnumFontFamiliesEx(hdc, &lf, EnumFontFamExProc, (LPARAM)&typefaces, 0);

    ReleaseDC(0, hdc);
    return typefaces;
}

int CALLBACK EnumFontFamExProc(const LOGFONT *lpelfe, const TEXTMETRIC *lpntme, DWORD FontType, LPARAM lParam)
{
    std::vector<std::wstring> *typefaces = (std::vector<std::wstring>*)lParam;
    typefaces->push_back(lpelfe->lfFaceName);
    return TRUE;
}

bool retrieveDateSettings(HWND hwnd)
{
    std::wstring str;
    FontInfo newFont;
    Color newColor;
    bool newList;

    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_FONT_NAME)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_FONT_NAME), &str[0], str.size() + 1);
    if (std::find(g_typefaces.begin(), g_typefaces.end(), str) == g_typefaces.end()) {
        MessageBox(hwnd, L"Η επιλεγμένη γραμματοσειρά δεν υποστηρίζεται", L"Σφάλμα", MB_ICONERROR | MB_OK);
        return false;
    }
    newFont.typeface = utf16_to_utf8(str);

    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_FONT_SIZE)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_FONT_SIZE), &str[0], str.size() + 1);
    newFont.size = std::stoi(str);
    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_FONT_WEIGHT)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_FONT_WEIGHT), &str[0], str.size() + 1);
    newFont.weight = std::stoi(str);
    if (newFont.weight <= 0 || newFont.weight > 1000) {
        MessageBox(hwnd, L"Η ένταση πρέπει να είναι από 1 μέχρι και 1000", L"Σφάλμα", MB_ICONERROR | MB_OK);
        return false;
    }

    newFont.italic = IsDlgButtonChecked(hwnd, IDCK_ITALIC);
    newFont.underlined = IsDlgButtonChecked(hwnd, IDCK_UNDERLINE);
    newFont.strikeout = IsDlgButtonChecked(hwnd, IDCK_STRIKEOUT);
    newList = IsDlgButtonChecked(hwnd, IDCK_LIST);

    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_COLOR_RED)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_COLOR_RED), &str[0], str.size() + 1);
    int r = std::stoi(str);
    if (r < 0 || r > 255) {
        MessageBox(hwnd, L"Το κόκκινο πρέπει να είναι από 0 μέχρι και 255", L"Σφάλμα", MB_ICONERROR | MB_OK);
        return false;
    }
    newColor.r = r;

    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_COLOR_GREEN)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_COLOR_GREEN), &str[0], str.size() + 1);
    int g = std::stoi(str);
    if (g < 0 || g > 255) {
        MessageBox(hwnd, L"Το πράσινο πρέπει να είναι από 0 μέχρι και 255", L"Σφάλμα", MB_ICONERROR | MB_OK);
        return false;
    }
    newColor.g = g;

    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_COLOR_BLUE)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_COLOR_BLUE), &str[0], str.size() + 1);
    int b = std::stoi(str);
    if (b < 0 || b > 255) {
        MessageBox(hwnd, L"Το μπλε πρέπει να είναι από 0 μέχρι και 255", L"Σφάλμα", MB_ICONERROR | MB_OK);
        return false;
    }
    newColor.b = b;

    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_COLOR_ALPHA)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_COLOR_ALPHA), &str[0], str.size() + 1);
    int a = std::stoi(str);
    if (a < 0 || a > 255) {
        MessageBox(hwnd, L"Η διαφάνεια πρέπει να είναι από 0 μέχρι και 255", L"Σφάλμα", MB_ICONERROR | MB_OK);
        return false;
    }
    newColor.a = a;

    if (newFont != g_config.defaultFont) {
        g_setDate.setFont(newFont);
    }

    tm curTM = {
        0, 0, 0, g_setDate.date.day, g_setDate.date.month - 1, g_setDate.date.year - 1900, 0, 0, 0
    };
    time_t curTime = mktime(&curTM);
    curTM = *localtime(&curTime);

    if (((curTM.tm_wday == 0 || curTM.tm_wday == 6) && newColor != g_config.weekendColor) ||
        ((curTM.tm_wday > 0 && curTM.tm_wday < 6) && newColor != g_config.defaultColor)) {
        g_setDate.setColor(newColor);
    }
    g_setDate.setListed(newList);

    return true;
}

HWND CreateToolTip(int toolID, HWND hDlg, const wchar_t* pszText)
{
    if (!toolID || !hDlg || !pszText)
    {
        return FALSE;
    }
    // Get the window of the tool.
    HWND hwndTool = GetDlgItem(hDlg, toolID);

    // Create the tooltip.
    HWND hwndTip = CreateWindowEx(0, TOOLTIPS_CLASS, NULL,
                              WS_POPUP |TTS_ALWAYSTIP | TTS_BALLOON,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              hDlg, NULL,
                              GetModuleHandle(0), NULL);

   if (!hwndTool || !hwndTip)
   {
       return (HWND)NULL;
   }

    // Associate the tooltip with the tool.
    TOOLINFO toolInfo = { 0 };
    toolInfo.cbSize = sizeof(toolInfo);
    toolInfo.hwnd = hDlg;
    toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
    toolInfo.uId = (UINT_PTR)hwndTool;
    toolInfo.lpszText = (LPWSTR)pszText;
    SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

    return hwndTip;
}

bool retrieveSettings(HWND hwnd)
{
    std::wstring str;

    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_FONT_NAME)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_FONT_NAME), &str[0], str.size() + 1);
    if (std::find(g_typefaces.begin(), g_typefaces.end(), str) == g_typefaces.end()) {
        MessageBox(hwnd, L"Η επιλεγμένη γραμματοσειρά δεν υποστηρίζεται", L"Σφάλμα", MB_ICONERROR | MB_OK);
        return false;
    }
    g_config.defaultFont.typeface = utf16_to_utf8(str);

    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_FONT_SIZE)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_FONT_SIZE), &str[0], str.size() + 1);
    g_config.defaultFont.size = std::stoi(str);
    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_NUMSIZE)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_NUMSIZE), &str[0], str.size() + 1);
    g_config.numberSize = std::stoi(str);
    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_FONT_WEIGHT)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_FONT_WEIGHT), &str[0], str.size() + 1);
    g_config.defaultFont.weight = std::stoi(str);
    if (g_config.defaultFont.weight <= 0 || g_config.defaultFont.weight > 1000) {
        MessageBox(hwnd, L"Η ένταση πρέπει να είναι από 1 μέχρι και 1000", L"Σφάλμα", MB_ICONERROR | MB_OK);
        return false;
    }

    g_config.defaultFont.italic = IsDlgButtonChecked(hwnd, IDCK_ITALIC);
    g_config.defaultFont.underlined = IsDlgButtonChecked(hwnd, IDCK_UNDERLINE);
    g_config.defaultFont.strikeout = IsDlgButtonChecked(hwnd, IDCK_STRIKEOUT);

    static const unsigned colorIdents[] = {
        ID_COLOR_RED_DEFAULT, ID_COLOR_GREEN_DEFAULT, ID_COLOR_BLUE_DEFAULT, ID_COLOR_ALPHA_DEFAULT,
        ID_COLOR_RED_WEEKEND, ID_COLOR_GREEN_WEEKEND, ID_COLOR_BLUE_WEEKEND, ID_COLOR_ALPHA_WEEKEND,
        ID_COLOR_RED_MONTH, ID_COLOR_GREEN_MONTH, ID_COLOR_BLUE_MONTH, ID_COLOR_ALPHA_MONTH,
        ID_COLOR_RED_DAY, ID_COLOR_GREEN_DAY, ID_COLOR_BLUE_DAY, ID_COLOR_ALPHA_DAY
    };
    static const wchar_t *colorTexts[] = {
        L"Το γενικό κόκκινο", L"Το γενικό πράσινο", L"Το γενικό μπλε", L"Η γενική διαφάνεια",
        L"Το κόκκινο σαββατοκύριακου", L"Το πράσινο σαββατοκύριακου", L"Το μπλε σαββατοκύριακου", L"Η διαφάνεια σαββατοκύριακου",
        L"Το κόκκινο μηνός", L"Το πράσινο μηνός", L"Το μπλε μηνός", L"Η διαφάνεια μηνός",
        L"Το κόκκινο ημέρας", L"Το πράσινο ημέρας", L"Το μπλε ημέρας", L"Η διαφάνεια ημέρας"
    };
    int *colorPtrs[] = {
        &g_config.defaultColor.r, &g_config.defaultColor.g, &g_config.defaultColor.b, &g_config.defaultColor.a,
        &g_config.weekendColor.r, &g_config.weekendColor.g, &g_config.weekendColor.b, &g_config.weekendColor.a,
        &g_config.monthColor.r, &g_config.monthColor.g, &g_config.monthColor.b, &g_config.monthColor.a,
        &g_config.curDayColor.r, &g_config.curDayColor.g, &g_config.curDayColor.b, &g_config.curDayColor.a
    };

    for (int i = 0; i < 16; ++i) {
        str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, colorIdents[i])));
        ComboBox_GetText(GetDlgItem(hwnd, colorIdents[i]), &str[0], str.size() + 1);
        int color = std::stoi(str);
        if (color < 0 || color > 255) {
            MessageBox(hwnd, (std::wstring(colorTexts[i]) + L" πρέπει να είναι από 0 μέχρι και 255").c_str(), L"Σφάλμα", MB_ICONERROR | MB_OK);
            return false;
        }
        *colorPtrs[i] = color;
    }

    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_MARGIN_NARROW)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_MARGIN_NARROW), &str[0], str.size() + 1);
    g_config.marginNarrow = std::stoi(str);
    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_MARGIN_WIDE)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_MARGIN_WIDE), &str[0], str.size() + 1);
    g_config.marginWide = std::stoi(str);

    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_MARGIN_LEFT)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_MARGIN_LEFT), &str[0], str.size() + 1);
    g_config.marginLeft = std::stoi(str);
    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_MARGIN_TOP)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_MARGIN_TOP), &str[0], str.size() + 1);
    g_config.marginTop = std::stoi(str);
    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_MARGIN_RIGHT)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_MARGIN_RIGHT), &str[0], str.size() + 1);
    g_config.marginRight = std::stoi(str);
    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_MARGIN_BOTTOM)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_MARGIN_BOTTOM), &str[0], str.size() + 1);
    g_config.marginBottom = std::stoi(str);

    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_TITLE_SIZE)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_TITLE_SIZE), &str[0], str.size() + 1);
    g_config.titleSize = std::stoi(str);
    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_HEADER_SIZE)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_HEADER_SIZE), &str[0], str.size() + 1);
    g_config.headerIndexSize = std::stoi(str);
    str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, ID_WEEKS)));
    ComboBox_GetText(GetDlgItem(hwnd, ID_WEEKS), &str[0], str.size() + 1);
    g_config.rowAmount = std::stoi(str);
    if (g_config.rowAmount <= 0) {
        MessageBox(hwnd, L"Το πλήθος εβδομάδων πρέπει να είναι μεγαλύτερο του μηδενός", L"Σφάλμα", MB_ICONERROR | MB_OK);
        return false;
    }

    static const wchar_t *days[] = {
        L"Δευτέρας", L"Τρίτης", L"Τετάρτης", L"Πέμπτης", L"Παρασκευής", L"Σαββάτου", L"Κυριακής"
    };
    static const unsigned idents[] = {
        ID_PERCENT_MON, ID_PERCENT_TUE, ID_PERCENT_WED, ID_PERCENT_THU, ID_PERCENT_FRI, ID_PERCENT_SAT, ID_PERCENT_SUN
    };
    float sizeSum = 0;
    for (int i = 0; i < 7; ++i) {
        str.resize(ComboBox_GetTextLength(GetDlgItem(hwnd, idents[i])));
        ComboBox_GetText(GetDlgItem(hwnd, idents[i]), &str[0], str.size() + 1);
        float percent;
        try {
            percent = std::stof(str);
        } catch (...) {
            MessageBox(hwnd, (std::wstring(L"Το μέγεθος ") + days[i] + L" πρέπει να είναι αριθμός").c_str(), L"Σφάλμα", MB_ICONERROR | MB_OK);
            return false;
        }

        if (percent <= 0) {
            MessageBox(hwnd, (std::wstring(L"Το μέγεθος ") + days[i] + L" πρέπει να είναι μεγαλύτερο του μηδενός").c_str(), L"Σφάλμα", MB_ICONERROR | MB_OK);
            return false;
        }
        g_config.columnSizes[i] = percent;
        sizeSum += percent;
    }

    for (int i = 0; i < 7; ++i) {
        g_config.columnSizes[i] /= sizeSum;
    }

    return true;
}