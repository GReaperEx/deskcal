#include "TextUtils.h"

#include <windows.h>

std::string utf16_to_utf8(const std::wstring& wstr)
{
    std::string str;

    size_t reqSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    if (reqSize > 0) {
        str.resize(reqSize);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], reqSize, NULL, NULL);
    }

    return str;
}

std::wstring utf8_to_utf16(const std::string& str)
{
    std::wstring wstr;

    size_t reqSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    if (reqSize > 0) {
        wstr.resize(reqSize);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], reqSize);
    }

    return wstr;
}