#include "DeskCalendar.h"
#include "TextUtils.h"

#include <cstdio>
#include <ctime>
#include <cmath>

void DeskCalendar::render(HWND hwnd)
{
    RECT workArea;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
    workArea.left += _marginLeft;
    workArea.top += _marginTop;
    workArea.right -= _marginRight;
    workArea.bottom -= _marginBottom;

    int screenX = workArea.left;
    int screenY = workArea.top;
    int screenW = workArea.right - workArea.left;
    int screenH = workArea.bottom - workArea.top;
    SetWindowPos(hwnd, HWND_NOTOPMOST, screenX, screenY, screenW, screenH, SWP_SHOWWINDOW);

    WBitmap canvas(screenW, screenH, WBitmap::BGRA(0, 0, 0, 0));
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

    int titleX = _headerIndexSize + _marginWide;
    int titleY = 0;
    int titleW = screenW - _headerIndexSize - _marginWide;
    int titleH = _titleSize;

    CalTitle curTitle(toDate, titleX, titleY, titleW, titleH);

    float Y = 0, X = _headerIndexSize + _marginWide;
    curTitle.renderGraphics(canvas, _defaultColor);
    Y += titleH + _marginWide;

    for (int i = 0; i < 7; ++i) {
        int boxH = _headerIndexSize;
        float boxW = titleW*_columnSizes[i] - _marginNarrow;

        CalHeader curHeader(days[i], std::round(X), std::round(Y), std::round(boxW), boxH);
        curHeader.renderGraphics(canvas, _defaultColor);

        X += boxW + _marginNarrow;
    }
    Y += _headerIndexSize + _marginWide;

    for (int i = 0; i < _rowAmount; ++i) {
        int boxH = (screenH - (_rowAmount - 1)*_marginNarrow - 3*_headerIndexSize - 2*_marginWide)/_rowAmount;
        X = 0;

        CalIndex curIndex(i + 1, std::round(X), std::round(Y), _headerIndexSize, boxH);
        curIndex.renderGraphics(canvas, _defaultColor);
        X = _headerIndexSize + _marginWide;

        for (int j = 0; j < 7; ++j) {
            float boxW = titleW*_columnSizes[j] - _marginNarrow;

            CalDate curDate(CalDate::Date(2020, (i*7 + j + 30)/31 + 8, (i*7 + j + 30) % 31 + 1), L"", _defaultColor, _defaultFont);
            if (curDate.date.day == 1) {
                CalHeader monHeader(L"", std::round(X), std::round(Y), std::round(boxW), _headerIndexSize);
                monHeader.renderGraphics(canvas, _monthColor);
                Y += _headerIndexSize + _marginNarrow;
                boxH -= _headerIndexSize + _marginNarrow;

                if (curDate.date == today) {
                    WBitmap perimeter(std::round(boxW) + _marginNarrow*2, boxH + _marginNarrow*2, _curDayColor);
                    WBitmap clipInside(std::round(boxW), boxH, Color());
                    clipInside.renderOnBmp(perimeter, _marginNarrow, _marginNarrow, false);
                    perimeter.renderOnBmp(canvas, std::round(X - _marginNarrow), std::round(Y - _marginNarrow));
                }

                if (j > 4) {
                    curDate.setColor(_weekendColor);
                }
                curDate.renderGraphics(canvas, std::round(X), std::round(Y), std::round(boxW), boxH);

                X += boxW + _marginNarrow;
                Y -= _headerIndexSize + _marginNarrow;
                boxH += _headerIndexSize + _marginNarrow;
            } else {
                if (curDate.date == today) {
                    WBitmap perimeter(boxW + _marginNarrow*2, boxH + _marginNarrow*2, _curDayColor);
                    WBitmap clipInside(boxW, boxH, Color());
                    clipInside.renderOnBmp(perimeter, _marginNarrow, _marginNarrow, false);
                    perimeter.renderOnBmp(canvas, std::round(X - _marginNarrow), std::round(Y - _marginNarrow));
                }

                if (j > 4) {
                    curDate.setColor(_weekendColor);
                }
                curDate.renderGraphics(canvas, std::round(X), std::round(Y), std::round(boxW), boxH);

                X += boxW + _marginNarrow;
            }
        }
        Y += boxH + _marginNarrow;
    }
    canvas.renderOnWnd(hwnd);

    Y = 0;
    X = _headerIndexSize + _marginWide;
    curTitle.renderText(hwnd, _defaultFont);
    Y += titleH + _marginWide;

    for (int i = 0; i < 7; ++i) {
        int boxH = _headerIndexSize;
        int boxW = std::round(titleW*_columnSizes[i]) - _marginNarrow;

        CalHeader curHeader(days[i], std::round(X), std::round(Y), std::round(boxW), boxH);
        curHeader.renderText(hwnd, _defaultFont);

        X += boxW + _marginNarrow;
    }
    Y += _headerIndexSize + _marginWide;

    for (int i = 0; i < _rowAmount; ++i) {
        int boxH = (screenH - (_rowAmount - 1)*_marginNarrow - 3*_headerIndexSize - 2*_marginWide)/_rowAmount;
        X = 0;

        CalIndex curIndex(i + 1, std::round(X), std::round(Y), _headerIndexSize, boxH);
        curIndex.renderText(hwnd, _numberSize);
        X = _headerIndexSize + _marginWide;

        for (int j = 0; j < 7; ++j) {
            int boxW = std::round(titleW*_columnSizes[j]) - _marginNarrow;

            CalDate curDate(CalDate::Date(2020, (i*7 + j + 30)/31 + 8, (i*7 + j + 30) % 31 + 1), L"1 Τοστ, 2 Τοστ, 3 Τοστ, Χόρτασα!", _defaultColor, _defaultFont);
            if (curDate.date.day == 1) {
                CalHeader monHeader(months[0][curDate.date.month - 1], std::round(X), std::round(Y), std::round(boxW), _headerIndexSize);
                monHeader.renderText(hwnd, _defaultFont);
                Y += _headerIndexSize + _marginNarrow;
                boxH -= _headerIndexSize + _marginNarrow;

                if (j > 4) {
                    curDate.setColor(_weekendColor);
                }
                curDate.renderText(hwnd, std::round(X), std::round(Y), std::round(boxW), boxH, _numberSize);

                X += boxW + _marginNarrow;
                Y -= _headerIndexSize + _marginNarrow;
                boxH += _headerIndexSize + _marginNarrow;
            } else {
                curDate.renderText(hwnd, std::round(X), std::round(Y), std::round(boxW), boxH, _numberSize);
                X += boxW + _marginNarrow;
            }
        }
        Y += boxH + _marginNarrow;
    }
}

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
                _headerIndexSize = newVal;
            } else if (input == "titleSize") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _titleSize = newVal;
            } else if (input == "marginNarrow") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _marginNarrow = newVal;
            } else if (input == "marginWide") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _marginWide = newVal;
            } else if (input == "marginLeft") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _marginLeft = newVal;
            } else if (input == "marginTop") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _marginTop = newVal;
            } else if (input == "marginRight") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _marginRight = newVal;
            } else if (input == "marginBottom") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _marginBottom = newVal;
            } else if (input == "numberSize") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _numberSize = newVal;
            } else if (input == "defaultColor") {
                line >> _defaultColor;
            } else if (input == "weekendColor") {
                line >> _weekendColor;
            } else if (input == "monthColor") {
                line >> _monthColor;
            } else if (input == "curDayColor") {
                line >> _curDayColor;
            } else if (input == "defaultFont") {
                line >> _defaultFont;
            } else if (input == "columnSizes") {
                float sizes[7];
                if (!(line >> c >> sizes[0] >> c >> sizes[1] >> c >> sizes[2] >> c >> sizes[3] >> c >> sizes[4] >> c >> sizes[5] >> c >> sizes[6] >> c)) {
                    continue;
                }
                for (int i = 0; i < 7; ++i) {
                    _columnSizes[i] = sizes[i];
                }
            } else if (input == "rowAmount") {
                int newVal;
                if (!(line >> newVal) || newVal <= 0) {
                    continue;
                }
                _rowAmount = newVal;
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
    outfile << "headerIndexSize = " << _headerIndexSize << std::endl;
    outfile << "// Height of Title-bar" << std::endl;
    outfile << "titleSize = " << _titleSize << std::endl;
    outfile << std::endl;
    outfile << "// Space between calendar cells" << std::endl;
    outfile << "marginNarrow = " << _marginNarrow << std::endl;
    outfile << "// Space between calendar cell area and headers/indices, also between headers and title-bar" << std::endl;
    outfile << "marginWide = " << _marginWide << std::endl;
    outfile << std::endl;
    outfile << "// Space between the left side of the screen and the window" << std::endl;
    outfile << "marginLeft = " << _marginLeft << std::endl;
    outfile << "// Space between the top side of the screen and the window" << std::endl;
    outfile << "marginTop = " << _marginTop << std::endl;
    outfile << "// Space between the right side of the screen and the window" << std::endl;
    outfile << "marginRight = " << _marginRight << std::endl;
    outfile << "// Space between the bottom side of the screen (not including task-bar) and the window" << std::endl;
    outfile << "marginBottom = " << _marginBottom << std::endl;
    outfile << std::endl;
    outfile << "// Default color of all blocks, except weekend cells and month headers" << std::endl;
    outfile << "defaultColor = " << _defaultColor << std::endl;
    outfile << "// Default color of all weekend cells" << std::endl;
    outfile << "weekendColor = " << _weekendColor << std::endl;
    outfile << "// Color of all month headers" << std::endl;
    outfile << "monthColor = " << _monthColor << std::endl;
    outfile << "// Current day highlight color" << std::endl;
    outfile << "curDayColor = " << _curDayColor << std::endl;
    outfile << std::endl;
    outfile << "// Default font of all blocks, except cell numbers and indices" << std::endl;
    outfile << "// (TypeFace, Size, Weight/Boldness[1, 1000], isItalic[0, 1], isUnderlined[0, 1], isStrikeout[0, 1])" << std::endl;
    outfile << "defaultFont = " << _defaultFont << std::endl;
    outfile << "// Font size for cell and index numbers" << std::endl;
    outfile << "numberSize = " << _numberSize << std::endl;
    outfile << std::endl;
    outfile << "// Proportional widths for all 7 columns, must add up to 1" << std::endl;
    outfile << "columnSizes = " << '(' << _columnSizes[0] << ", " << _columnSizes[1] << ", " << _columnSizes[2] << ", " << _columnSizes[3] << ", "
                                       << _columnSizes[4] << ", " << _columnSizes[5] << ", " << _columnSizes[6] << ')' << std::endl;
    outfile << "// Maximum amount of rows on screen" << std::endl;
    outfile << "rowAmount = " << _rowAmount << std::endl;

    outfile.close();
    remove("deskcal.cfg");
    rename("deskcal.cfg.tmp", "deskcal.cfg");

    return true;
}

void DeskCalendar::resetConfig()
{
    _headerIndexSize = 24;
    _titleSize = 40;

    _marginNarrow = 2;
    _marginWide = 6;

    _marginLeft = 100;
    _marginTop = 50;
    _marginRight = 50;
    _marginBottom = 50;

    _defaultColor = Color(0, 0, 255, 128);
    _weekendColor = Color(255, 192, 0, 128);
    _monthColor = Color(255, 0, 0, 128);
    _curDayColor = Color(255, 0, 0, 255);

    _defaultFont = FontInfo{"Times New Roman", 18, 400, 0, 0, 0};
    _numberSize = 18;

    for (int i = 0; i < 7; ++i) {
        _columnSizes[i] = 0.142857;
    }
    _rowAmount = 5;
}

bool DeskCalendar::loadDates()
{
    std::ifstream infile("deskcal.sav");
    if (!infile.is_open()) {
        return false;
    }

    std::string lineStr;
    while (std::getline(infile, lineStr)) {

    }

    return true;
}

bool DeskCalendar::saveDates() const
{

}
