#ifndef TEXT_UTILS_H
#define TEXT_UTILS_H

#include <string>

std::string utf16_to_utf8(const std::wstring& wstr);
std::wstring utf8_to_utf16(const std::string& str);

#endif // TEXT_UTILS_H