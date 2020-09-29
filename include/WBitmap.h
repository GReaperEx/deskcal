#ifndef WBITMAP_H
#define WBITMAP_H

#include <windows.h>
#include <string>

#include <iostream>
#include <vector>
#include <cassert>

#include <png++/png.hpp>

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
    WBitmap() {}

    WBitmap(int width, int height, const std::vector<BGRA>& pixelData)
    : _hbmp(0) {
        set(width, height, pixelData);
    }

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
    void set(int width, int height, const std::vector<BGRA>& pixelData);

    void renderOnBmp(WBitmap& other, int x, int y, bool blend = true, int w = 0, int h = 0) const;
    void renderOnWnd(HWND hwnd, int x = 0, int y = 0) const;

private:
    HBITMAP _hbmp;
    int _width;
    int _height;
};

#endif // WBITMAP_H
