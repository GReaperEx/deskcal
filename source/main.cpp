#include "WBitmap.h"
#include "DeskCalendar.h"

#include <dwmapi.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HRESULT ExtendIntoClientAll(HWND);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
    static wchar_t szWindowClass[] = L"Desktop Calendar";

    WNDCLASSEX wcex;
    wcex.cbSize         = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
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

    DeskCalendar myCalendar;
    myCalendar.render(hWnd);


    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, message, wParam, lParam);
}

HRESULT ExtendIntoClientAll(HWND hwnd)
{
    MARGINS margins = { -1, -1, -1, -1 };
    return DwmExtendFrameIntoClientArea(hwnd, &margins);
}
