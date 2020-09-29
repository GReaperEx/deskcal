#include "WBitmap.h"
#include "TextUtils.h"

void WBitmap::reload(const std::wstring& filePath) {
    if (_hbmp) {
        DeleteObject(_hbmp);
        _hbmp = 0;
    }

    std::wstring ext = filePath.substr(filePath.size() - 4);
    if (ext == L".bmp") {
        _hbmp = (HBITMAP) LoadImage( NULL, filePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    } else if (ext == L".png") {
        png::image<png::rgba_pixel> pngImage(utf16_to_utf8(filePath), png::require_color_space<png::rgba_pixel>());

        std::vector<BGRA> pixelData(pngImage.get_width() * pngImage.get_height());
        for (unsigned i = 0; i < pngImage.get_height(); ++i) {
            for (unsigned j = 0; j < pngImage.get_width(); ++j) {
                int index = i*pngImage.get_width() + j;
                int r = pngImage[i][j].red;
                int g = pngImage[i][j].green;
                int b = pngImage[i][j].blue;
                int a = pngImage[i][j].alpha;

                pixelData[index] = BGRA(r, g, b, a);
            }
        }
        set(pngImage.get_width(), pngImage.get_height(), pixelData);
    }
}

void WBitmap::resize(int width, int height, WBitmap::BGRA color) {
    if (_hbmp) {
        DeleteObject(_hbmp);
        _hbmp = 0;
    }

    int rowSize = width*(32/8);
    rowSize += (4 - rowSize % 4) % 4;

    BGRA *pixels = new BGRA[rowSize*height];
    for (int i = 0; i < rowSize*height; ++i) {
        pixels[i] = color;
    }

    BITMAP bmp;
    bmp.bmType = 0;
    bmp.bmWidth = width;
    bmp.bmHeight = height;
    bmp.bmWidthBytes = rowSize;
    bmp.bmPlanes = 1;
    bmp.bmBitsPixel = 32;
    bmp.bmBits = (LPVOID)pixels;

    _hbmp = CreateBitmapIndirect(&bmp);

    _width = width;
    _height = height;
    delete [] pixels;
}

void WBitmap::set(int width, int height, const std::vector<BGRA>& pixelData)
{
    if (_hbmp) {
        DeleteObject(_hbmp);
        _hbmp = 0;
    }

    BITMAP bmp;
    bmp.bmType = 0;
    bmp.bmWidth = width;
    bmp.bmHeight = height;
    bmp.bmWidthBytes = width*4;
    bmp.bmPlanes = 1;
    bmp.bmBitsPixel = 32;
    bmp.bmBits = (LPVOID)&pixelData[0];

    _hbmp = CreateBitmapIndirect(&bmp);

    _width = width;
    _height = height;
}

void WBitmap::renderOnBmp(WBitmap& other, int x, int y, bool blend, int w, int h) const {
    if (_hbmp) {
        HDC hdc = GetDC(0);
        HDC dstDC = CreateCompatibleDC(hdc);
        SelectObject(dstDC, other._hbmp);
        HDC srcDC = CreateCompatibleDC(hdc);
        SelectObject(srcDC, _hbmp);

        if (w == 0) {
            w = _width;
        }
        if (h == 0) {
            h = _height;
        }

        SetStretchBltMode(srcDC, HALFTONE);
        SetBrushOrgEx(srcDC, 0, 0, NULL);

        if (blend) {
            AlphaBlend(dstDC, x, y, w, h,
                       srcDC, 0, 0, _width, _height,
                       BLENDFUNCTION{AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
        } else {
            StretchBlt(dstDC, x, y, w, h,
                       srcDC, 0, 0, _width, _height, SRCCOPY);
        }

        DeleteDC(dstDC);
        DeleteDC(srcDC);
        ReleaseDC(0, hdc);
    }
}

void WBitmap::renderOnWnd(HWND hwnd, int x, int y) const {
    if (_hbmp) {
        HDC wndDC = GetDC(hwnd);
        HDC bmpDC = CreateCompatibleDC(wndDC);
        SelectObject(bmpDC, _hbmp);

        BitBlt(wndDC, x, y, _width, _height, bmpDC, 0, 0, SRCCOPY);

        DeleteDC(bmpDC);
        ReleaseDC(hwnd, wndDC);
    }
}
