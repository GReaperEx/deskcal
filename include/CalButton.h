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
        int marginX = (newW - _bitmap.getWidth())/2;
        int marginY = (newH - _bitmap.getHeight())/2;

        _x = newX + marginX;
        _y = newY + marginY;
        _w = _bitmap.getWidth();
        _h = _bitmap.getHeight();

        _pressed = false;
    }

    bool onLMBdown(int x, int y) {
        if (_x <= x && x < _x + _w &&
            _y <= y && y < _y + _h) {
            _pressed = true;
            return true;
        }
        return false;
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

    void renderOnWnd(HWND hwnd, Color color) {
        WBitmap layer(_w, _h, color);
        _prevColor = color;
        _bitmap.renderOnBmp(layer, 0, 0);
        layer.renderOnWnd(hwnd, _x, _y);
    }

    void maskOnWnd(HWND hwnd) const {
        WBitmap layer(_w, _h, _prevColor);
        layer.renderOnWnd(hwnd, _x, _y);
    }

    void setPrevColor(Color newColor) {
        _prevColor = newColor;
    }

private:
    bool _pressed;
    Color _prevColor;

    WBitmap _bitmap;
    int _x;
    int _y;
    int _w;
    int _h;
};

#endif //
