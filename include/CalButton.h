#ifndef CAL_BUTTON_H
#define CAL_BUTTON_H

#include "WBitmap.h"

class CalButton
{
public:
    CalButton() : _pressed(false) {}
    CalButton(const std::wstring& bmpPath)
    : _pressed(false), _bitmap(bmpPath)
    {}

    void load(const std::wstring& bmpPath) {
        _bitmap.reload(bmpPath);
    }

    void update(int newX, int newY, int newW, int newH) {
        int marginX = (newW - 32)/2;
        int marginY = (newH - 32)/2;

        _x = newX + marginX;
        _y = newY + marginY;
        _w = 32;
        _h = 32;
    }

    void onLMBdown(int x, int y) {
        if (_x <= x && x < _x + _w &&
            _y <= y && y < _y + _h) {
            _pressed = true;
        }
    }

    bool onLMBup(int x, int y) {
        bool wasClicked = false;
        if (_x <= x && x < _x + _w &&
            _y <= y && y < _y + _h && _pressed) {
            wasClicked = true;
        }
        _pressed = false;

        return wasClicked;
    }

    void renderOnBmp(WBitmap& canvas) const {
        _bitmap.renderOnBmp(canvas, _x, _y, true, _w, _h);
    }

private:
    bool _pressed;

    WBitmap _bitmap;
    int _x;
    int _y;
    int _w;
    int _h;
};

#endif //