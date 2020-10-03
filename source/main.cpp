#include "WBitmap.h"
#include "DeskCalendar.h"

#include <dwmapi.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT ExtendIntoClientAll(HWND);

DeskCalendar* calendarPtr;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    static wchar_t szWindowClass[] = L"Desktop Calendar";

    WNDCLASSEX wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = 0;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = 0;
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, L"Call to RegisterClassEx failed!", L"Desktop Calendar", 0);
        return 1;
    }

    static wchar_t szTitle[] = L"Desktop Calendar";

    HWND hWnd = CreateWindowEx(
        0,
        szWindowClass,
        szTitle,
        WS_POPUP,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    if (!hWnd) {
        MessageBox(NULL, L"Call to CreateWindow failed!", L"Desktop Calendar", 0);
        return 1;
    }
    ExtendIntoClientAll(hWnd);

    DeskCalendar myCalendar(hWnd);
    calendarPtr = &myCalendar;

    myCalendar.update();
    myCalendar.render();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_LBUTTONDBLCLK:
    case WM_CHAR:
        calendarPtr->handleInput(hWnd, message, wParam, lParam);
    break;
    case WM_ERASEBKGND:
    break;
    case WM_DESTROY:
        calendarPtr->update();
        calendarPtr->saveDates();
        PostQuitMessage(0);
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

HRESULT ExtendIntoClientAll(HWND hwnd)
{
    MARGINS margins = { -1, -1, -1, -1 };
    return DwmExtendFrameIntoClientArea(hwnd, &margins);
}
