#include "WBitmap.h"

void WBitmap::reload(const std::wstring& filePath) {
    if (_hbmp) {
        DeleteObject(_hbmp);
    }
    _hbmp = (HBITMAP) LoadImage( NULL, filePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

    BITMAP bm;
    GetObject(_hbmp, sizeof(bm), &bm);
    _width = bm.bmWidth;
    _height = bm.bmHeight;
}

void WBitmap::resize(int width, int height, BGRA color) {
    if (_hbmp) {
        DeleteObject(_hbmp);
    }

    BGRA *pixels = new BGRA[width*height];
    for (int i = 0; i < width*height; ++i) {
        pixels[i] = color;
    }

    HDC hdc = GetDC(0);
    BITMAPINFOHEADER infoHeader;
    BITMAPINFO bmpInfo;

    infoHeader.biBitCount = 32;
    infoHeader.biClrImportant = 0;
    infoHeader.biClrUsed = 0;
    infoHeader.biCompression = BI_RGB;
    infoHeader.biHeight = height;
    infoHeader.biPlanes = 1;
    infoHeader.biSize = sizeof(infoHeader);
    infoHeader.biSizeImage = width*height*4;
    infoHeader.biWidth = width;
    infoHeader.biXPelsPerMeter = 0;
    infoHeader.biYPelsPerMeter = 0;

    bmpInfo.bmiHeader = infoHeader;
    BYTE *dibPixels;
    _hbmp = CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, (void**)&dibPixels, NULL, 0);
    memcpy(dibPixels, pixels, width*height*4);
    delete [] pixels;

    _width = width;
    _height = height;
    ReleaseDC(0, hdc);
}

void WBitmap::renderOnBmp(WBitmap& other, int x, int y) {
    if (_hbmp) {
        HDC hdc = GetDC(0);
        HDC dstDC = CreateCompatibleDC(hdc);
        SelectObject(dstDC, other._hbmp);
        HDC srcDC = CreateCompatibleDC(hdc);
        SelectObject(srcDC, _hbmp);

        AlphaBlend(dstDC, x, y, _width, _height,
                   srcDC, 0, 0, _width, _height,
                   BLENDFUNCTION{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});

        DeleteDC(dstDC);
        DeleteDC(srcDC);
        ReleaseDC(0, hdc);
    }
}

void WBitmap::renderOnWnd(HWND hwnd) {
    if (_hbmp) {
        HDC wndDC = GetDC(hwnd);
        HDC bmpDC = CreateCompatibleDC(wndDC);
        SelectObject(bmpDC, _hbmp);

        BitBlt(wndDC, 0, 0, _width, _height, bmpDC, 0, 0, SRCCOPY);

        DeleteDC(bmpDC);
        ReleaseDC(hwnd, wndDC);
    }
}
