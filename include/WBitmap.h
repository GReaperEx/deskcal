#ifndef WBITMAP_H
#define WBITMAP_H

#include <windows.h>
#include <string>

#include <iostream>

#include <cassert>

class WBitmap
{
public:
    struct BGRA {
        BYTE b;
        BYTE g;
        BYTE r;
        BYTE a;

        BGRA() {}

        BGRA(int _r, int _g, int _b, int _a)
        : b(_b), g(_g), r(_r), a(_a) {
            b = a*b/255;
            g = a*g/255;
            r = a*r/255;
        }
    };

public:
    WBitmap(int width, int height, BGRA color = BGRA(0, 0, 0, 255))
    : _hbmp(0) {
        resize(width, height, color);
    }

    WBitmap(const std::wstring& filePath)
    : _hbmp(0) {
        reload(filePath);
    }

    ~WBitmap() {
        if (_hbmp) {
            DeleteObject(_hbmp);
        }
    }

    void reload(const std::wstring& filePath);
    void resize(int width, int height, BGRA color);

    void renderOnBmp(WBitmap& other, int x, int y);
    void renderOnWnd(HWND hwnd);

private:
    HBITMAP _hbmp;
    int _width;
    int _height;
};

#endif // WBITMAP_H
